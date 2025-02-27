INTERFACE [arm]:

#include "kip.h"

class Kip_init
{
public:
  static void init();
};


//---------------------------------------------------------------------------
IMPLEMENTATION [arm]:

#include <cstring>

#include "config.h"
#include "mem_layout.h"


// Make the stuff below apearing only in this compilation unit.
// Trick Preprocess to let the struct reside in the cc file rather
// than putting it into the _i.h file which is perfectly wrong in 
// this case.
namespace KIP_namespace
{
  enum
  {
    Num_mem_descs = 20,
    Max_len_version = 512,

    Size_mem_descs = sizeof(Mword) * 2 * Num_mem_descs,
  };

  struct KIP
  {
    Kip kip;
    char mem_descs[Size_mem_descs];
  };

  KIP my_kernel_info_page asm("my_kernel_info_page") __attribute__((section(".kernel_info_page"))) =
    {
      {
	/* 00/00  */ L4_KERNEL_INFO_MAGIC,
	             Config::Kernel_version_id,
	             (Size_mem_descs + sizeof(Kip)) >> 4,
	             {}, 0, {},
	/* 10/20  */ 0, {},
	/* 20/40  */ 0, 0, {},
	/* 30/60  */ 0, 0, {},
	/* 40/80  */ 0, 0, {},
	/* 50/A0  */ 0, (sizeof(Kip) << (sizeof(Mword)*4)) | Num_mem_descs, {},
	/* 60/C0  */ {},
	/* A0/140 */ 0, 0, 0, 0,
	/* B8/160 */ {},
	/* E0/1C0 */ 0, 0, {},
	/* F0/1D0 */ {"", 0, {{0, 0, 0, 0, 0, 0, {0}, 0, 0, {0}, {0}}}, {0}},
      },
      {}
    };
};

IMPLEMENT
void Kip_init::init()
{
  Kip *kinfo = reinterpret_cast<Kip*>(&KIP_namespace::my_kernel_info_page);
  Kip::init_global_kip(kinfo);
  kinfo->add_mem_region(Mem_desc(0, Mem_layout::User_max,
                        Mem_desc::Conventional, true));
  init_syscalls(kinfo);
}

//--------------------------------------------------------------
IMPLEMENTATION[64bit && !cpu_virt]:

PRIVATE static inline
void
Kip_init::init_syscalls(Kip *kinfo)
{
  union K
  {
    Kip k;
    Mword w[512];
  };

  K *k = reinterpret_cast<K *>(kinfo);
  k->w[0x100] = 0xd65f03c0d4000001; // svc #0; ret
}

//--------------------------------------------------------------
IMPLEMENTATION[64bit && cpu_virt]:

PRIVATE static inline
void
Kip_init::init_syscalls(Kip *kinfo)
{
  union K
  {
    Kip k;
    Mword w[512];
  };

  K *k = reinterpret_cast<K *>(kinfo);
  k->w[0x100] = 0xd65f03c0d4000002; // hvc #0; ret
}

//--------------------------------------------------------------
IMPLEMENTATION[32bit]:

PRIVATE static inline
void
Kip_init::init_syscalls(Kip *)
{}
