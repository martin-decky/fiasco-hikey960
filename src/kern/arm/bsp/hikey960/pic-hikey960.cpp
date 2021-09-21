//-------------------------------------------------------------------
IMPLEMENTATION [arm && pf_hikey960]:

#include "irq_mgr_multi_chip.h"
#include "gic_v2.h"
#include "panic.h"


PUBLIC static
void Pic::init_ap(Cpu_number cpu, bool resume)
{
  gic->init_ap(cpu, resume);
}

PUBLIC static FIASCO_INIT
void Pic::init()
{
  typedef Irq_mgr_multi_chip<9> Mgr;

  // assume GICv2
  gic = new Boot_object<Gic_v2>(Kmem::mmio_remap(Mem_layout::Gic_cpu_phys_base,
                                                 Gic_cpu_v2::Size),
                                Kmem::mmio_remap(Mem_layout::Gic_dist_phys_base,
                                                 Gic_dist::Size));

  Mgr *m = new Boot_object<Mgr>(1);
  m->add_chip(0, gic, gic->nr_irqs());
  Irq_mgr::mgr = m;
}
