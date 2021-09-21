IMPLEMENTATION [arm && mp && pf_hikey960]:

#include "config.h"
#include "koptions.h"

PUBLIC static
void
Platform_control::boot_ap_cpus(Address phys_tramp_mp_addr)
{
  for (unsigned i = 0; i < Config::Max_num_cpus; ++i)
    cpu_on(i, phys_tramp_mp_addr);
}
