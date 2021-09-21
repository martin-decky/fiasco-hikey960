INTERFACE [arm && pf_hikey960]: //---------------------------------------

EXTENSION class Mem_layout
{
public:
  enum Phys_layout_virt : Address
  {
    Gic_dist_phys_base   = 0xE82B1000,
    Gic_cpu_phys_base    = 0xE82B2000,
    Gic_h_phys_base      = 0xE82B4000,
    Gic_v_phys_base      = 0xE82B6000,
  };
};
