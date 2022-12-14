/* GNU/Linux/PowerPC specific low level interface, for the remote server for
   GDB.
   Copyright (C) 1995-2016 Free Software Foundation, Inc.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "server.h"
#include "linux-low.h"

#include <elf.h>
#define pt_regs __pt_regs
#include <asm/ptrace.h>
#undef pt_regs

#include "nat/ppc-linux.h"

static unsigned long ppc_hwcap;


/* Defined in auto-generated file powerpc-32l.c.  */
void init_registers_powerpc_32l (void);
extern const struct target_desc *tdesc_powerpc_32l;

/* Defined in auto-generated file powerpc-altivec32l.c.  */
void init_registers_powerpc_altivec32l (void);
extern const struct target_desc *tdesc_powerpc_altivec32l;

/* Defined in auto-generated file powerpc-cell32l.c.  */
void init_registers_powerpc_cell32l (void);
extern const struct target_desc *tdesc_powerpc_cell32l;

/* Defined in auto-generated file powerpc-vsx32l.c.  */
void init_registers_powerpc_vsx32l (void);
extern const struct target_desc *tdesc_powerpc_vsx32l;

/* Defined in auto-generated file powerpc-isa205-32l.c.  */
void init_registers_powerpc_isa205_32l (void);
extern const struct target_desc *tdesc_powerpc_isa205_32l;

/* Defined in auto-generated file powerpc-isa205-altivec32l.c.  */
void init_registers_powerpc_isa205_altivec32l (void);
extern const struct target_desc *tdesc_powerpc_isa205_altivec32l;

/* Defined in auto-generated file powerpc-isa205-vsx32l.c.  */
void init_registers_powerpc_isa205_vsx32l (void);
extern const struct target_desc *tdesc_powerpc_isa205_vsx32l;

/* Defined in auto-generated file powerpc-e500l.c.  */
void init_registers_powerpc_e500l (void);
extern const struct target_desc *tdesc_powerpc_e500l;

/* Defined in auto-generated file powerpc-64l.c.  */
void init_registers_powerpc_64l (void);
extern const struct target_desc *tdesc_powerpc_64l;

/* Defined in auto-generated file powerpc-altivec64l.c.  */
void init_registers_powerpc_altivec64l (void);
extern const struct target_desc *tdesc_powerpc_altivec64l;

/* Defined in auto-generated file powerpc-cell64l.c.  */
void init_registers_powerpc_cell64l (void);
extern const struct target_desc *tdesc_powerpc_cell64l;

/* Defined in auto-generated file powerpc-vsx64l.c.  */
void init_registers_powerpc_vsx64l (void);
extern const struct target_desc *tdesc_powerpc_vsx64l;

/* Defined in auto-generated file powerpc-isa205-64l.c.  */
void init_registers_powerpc_isa205_64l (void);
extern const struct target_desc *tdesc_powerpc_isa205_64l;

/* Defined in auto-generated file powerpc-isa205-altivec64l.c.  */
void init_registers_powerpc_isa205_altivec64l (void);
extern const struct target_desc *tdesc_powerpc_isa205_altivec64l;

/* Defined in auto-generated file powerpc-isa205-vsx64l.c.  */
void init_registers_powerpc_isa205_vsx64l (void);
extern const struct target_desc *tdesc_powerpc_isa205_vsx64l;

#define ppc_num_regs 73

#ifdef __powerpc64__
/* We use a constant for FPSCR instead of PT_FPSCR, because
   many shipped PPC64 kernels had the wrong value in ptrace.h.  */
static int ppc_regmap[] =
 {PT_R0 * 8,     PT_R1 * 8,     PT_R2 * 8,     PT_R3 * 8,
  PT_R4 * 8,     PT_R5 * 8,     PT_R6 * 8,     PT_R7 * 8,
  PT_R8 * 8,     PT_R9 * 8,     PT_R10 * 8,    PT_R11 * 8,
  PT_R12 * 8,    PT_R13 * 8,    PT_R14 * 8,    PT_R15 * 8,
  PT_R16 * 8,    PT_R17 * 8,    PT_R18 * 8,    PT_R19 * 8,
  PT_R20 * 8,    PT_R21 * 8,    PT_R22 * 8,    PT_R23 * 8,
  PT_R24 * 8,    PT_R25 * 8,    PT_R26 * 8,    PT_R27 * 8,
  PT_R28 * 8,    PT_R29 * 8,    PT_R30 * 8,    PT_R31 * 8,
  PT_FPR0*8,     PT_FPR0*8 + 8, PT_FPR0*8+16,  PT_FPR0*8+24,
  PT_FPR0*8+32,  PT_FPR0*8+40,  PT_FPR0*8+48,  PT_FPR0*8+56,
  PT_FPR0*8+64,  PT_FPR0*8+72,  PT_FPR0*8+80,  PT_FPR0*8+88,
  PT_FPR0*8+96,  PT_FPR0*8+104,  PT_FPR0*8+112,  PT_FPR0*8+120,
  PT_FPR0*8+128, PT_FPR0*8+136,  PT_FPR0*8+144,  PT_FPR0*8+152,
  PT_FPR0*8+160,  PT_FPR0*8+168,  PT_FPR0*8+176,  PT_FPR0*8+184,
  PT_FPR0*8+192,  PT_FPR0*8+200,  PT_FPR0*8+208,  PT_FPR0*8+216,
  PT_FPR0*8+224,  PT_FPR0*8+232,  PT_FPR0*8+240,  PT_FPR0*8+248,
  PT_NIP * 8,    PT_MSR * 8,    PT_CCR * 8,    PT_LNK * 8,
  PT_CTR * 8,    PT_XER * 8,    PT_FPR0*8 + 256,
  PT_ORIG_R3 * 8, PT_TRAP * 8 };
#else
/* Currently, don't check/send MQ.  */
static int ppc_regmap[] =
 {PT_R0 * 4,     PT_R1 * 4,     PT_R2 * 4,     PT_R3 * 4,
  PT_R4 * 4,     PT_R5 * 4,     PT_R6 * 4,     PT_R7 * 4,
  PT_R8 * 4,     PT_R9 * 4,     PT_R10 * 4,    PT_R11 * 4,
  PT_R12 * 4,    PT_R13 * 4,    PT_R14 * 4,    PT_R15 * 4,
  PT_R16 * 4,    PT_R17 * 4,    PT_R18 * 4,    PT_R19 * 4,
  PT_R20 * 4,    PT_R21 * 4,    PT_R22 * 4,    PT_R23 * 4,
  PT_R24 * 4,    PT_R25 * 4,    PT_R26 * 4,    PT_R27 * 4,
  PT_R28 * 4,    PT_R29 * 4,    PT_R30 * 4,    PT_R31 * 4,
  PT_FPR0*4,     PT_FPR0*4 + 8, PT_FPR0*4+16,  PT_FPR0*4+24,
  PT_FPR0*4+32,  PT_FPR0*4+40,  PT_FPR0*4+48,  PT_FPR0*4+56,
  PT_FPR0*4+64,  PT_FPR0*4+72,  PT_FPR0*4+80,  PT_FPR0*4+88,
  PT_FPR0*4+96,  PT_FPR0*4+104,  PT_FPR0*4+112,  PT_FPR0*4+120,
  PT_FPR0*4+128, PT_FPR0*4+136,  PT_FPR0*4+144,  PT_FPR0*4+152,
  PT_FPR0*4+160,  PT_FPR0*4+168,  PT_FPR0*4+176,  PT_FPR0*4+184,
  PT_FPR0*4+192,  PT_FPR0*4+200,  PT_FPR0*4+208,  PT_FPR0*4+216,
  PT_FPR0*4+224,  PT_FPR0*4+232,  PT_FPR0*4+240,  PT_FPR0*4+248,
  PT_NIP * 4,    PT_MSR * 4,    PT_CCR * 4,    PT_LNK * 4,
  PT_CTR * 4,    PT_XER * 4,    PT_FPSCR * 4,
  PT_ORIG_R3 * 4, PT_TRAP * 4
 };

static int ppc_regmap_e500[] =
 {PT_R0 * 4,     PT_R1 * 4,     PT_R2 * 4,     PT_R3 * 4,
  PT_R4 * 4,     PT_R5 * 4,     PT_R6 * 4,     PT_R7 * 4,
  PT_R8 * 4,     PT_R9 * 4,     PT_R10 * 4,    PT_R11 * 4,
  PT_R12 * 4,    PT_R13 * 4,    PT_R14 * 4,    PT_R15 * 4,
  PT_R16 * 4,    PT_R17 * 4,    PT_R18 * 4,    PT_R19 * 4,
  PT_R20 * 4,    PT_R21 * 4,    PT_R22 * 4,    PT_R23 * 4,
  PT_R24 * 4,    PT_R25 * 4,    PT_R26 * 4,    PT_R27 * 4,
  PT_R28 * 4,    PT_R29 * 4,    PT_R30 * 4,    PT_R31 * 4,
  -1,            -1,            -1,            -1,
  -1,            -1,            -1,            -1,
  -1,            -1,            -1,            -1,
  -1,            -1,            -1,            -1,
  -1,            -1,            -1,            -1,
  -1,            -1,            -1,            -1,
  -1,            -1,            -1,            -1,
  -1,            -1,            -1,            -1,
  PT_NIP * 4,    PT_MSR * 4,    PT_CCR * 4,    PT_LNK * 4,
  PT_CTR * 4,    PT_XER * 4,    -1,
  PT_ORIG_R3 * 4, PT_TRAP * 4
 };
#endif

static int
ppc_cannot_store_register (int regno)
{
  const struct target_desc *tdesc = current_process ()->tdesc;

#ifndef __powerpc64__
  /* Some kernels do not allow us to store fpscr.  */
  if (!(ppc_hwcap & PPC_FEATURE_HAS_SPE)
      && regno == find_regno (tdesc, "fpscr"))
    return 2;
#endif

  /* Some kernels do not allow us to store orig_r3 or trap.  */
  if (regno == find_regno (tdesc, "orig_r3")
      || regno == find_regno (tdesc, "trap"))
    return 2;

  return 0;
}

static int
ppc_cannot_fetch_register (int regno)
{
  return 0;
}

static void
ppc_collect_ptrace_register (struct regcache *regcache, int regno, char *buf)
{
  memset (buf, 0, sizeof (long));

  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    {
      /* Little-endian values always sit at the left end of the buffer.  */
      collect_register (regcache, regno, buf);
    }
  else if (__BYTE_ORDER == __BIG_ENDIAN)
    {
      /* Big-endian values sit at the right end of the buffer.  In case of
         registers whose sizes are smaller than sizeof (long), we must use a
         padding to access them correctly.  */
      int size = register_size (regcache->tdesc, regno);

      if (size < sizeof (long))
	collect_register (regcache, regno, buf + sizeof (long) - size);
      else
	collect_register (regcache, regno, buf);
    }
  else
    perror_with_name ("Unexpected byte order");
}

static void
ppc_supply_ptrace_register (struct regcache *regcache,
			    int regno, const char *buf)
{
  if (__BYTE_ORDER == __LITTLE_ENDIAN)
    {
      /* Little-endian values always sit at the left end of the buffer.  */
      supply_register (regcache, regno, buf);
    }
  else if (__BYTE_ORDER == __BIG_ENDIAN)
    {
      /* Big-endian values sit at the right end of the buffer.  In case of
         registers whose sizes are smaller than sizeof (long), we must use a
         padding to access them correctly.  */
      int size = register_size (regcache->tdesc, regno);

      if (size < sizeof (long))
	supply_register (regcache, regno, buf + sizeof (long) - size);
      else
	supply_register (regcache, regno, buf);
    }
  else
    perror_with_name ("Unexpected byte order");
}


#define INSTR_SC        0x44000002
#define NR_spu_run      0x0116

/* If the PPU thread is currently stopped on a spu_run system call,
   return to FD and ADDR the file handle and NPC parameter address
   used with the system call.  Return non-zero if successful.  */
static int
parse_spufs_run (struct regcache *regcache, int *fd, CORE_ADDR *addr)
{
  CORE_ADDR curr_pc;
  int curr_insn;
  int curr_r0;

  if (register_size (regcache->tdesc, 0) == 4)
    {
      unsigned int pc, r0, r3, r4;
      collect_register_by_name (regcache, "pc", &pc);
      collect_register_by_name (regcache, "r0", &r0);
      collect_register_by_name (regcache, "orig_r3", &r3);
      collect_register_by_name (regcache, "r4", &r4);
      curr_pc = (CORE_ADDR) pc;
      curr_r0 = (int) r0;
      *fd = (int) r3;
      *addr = (CORE_ADDR) r4;
    }
  else
    {
      unsigned long pc, r0, r3, r4;
      collect_register_by_name (regcache, "pc", &pc);
      collect_register_by_name (regcache, "r0", &r0);
      collect_register_by_name (regcache, "orig_r3", &r3);
      collect_register_by_name (regcache, "r4", &r4);
      curr_pc = (CORE_ADDR) pc;
      curr_r0 = (int) r0;
      *fd = (int) r3;
      *addr = (CORE_ADDR) r4;
    }

  /* Fetch instruction preceding current NIP.  */
  if ((*the_target->read_memory) (curr_pc - 4,
				  (unsigned char *) &curr_insn, 4) != 0)
    return 0;
  /* It should be a "sc" instruction.  */
  if (curr_insn != INSTR_SC)
    return 0;
  /* System call number should be NR_spu_run.  */
  if (curr_r0 != NR_spu_run)
    return 0;

  return 1;
}

static CORE_ADDR
ppc_get_pc (struct regcache *regcache)
{
  CORE_ADDR addr;
  int fd;

  if (parse_spufs_run (regcache, &fd, &addr))
    {
      unsigned int pc;
      (*the_target->read_memory) (addr, (unsigned char *) &pc, 4);
      return ((CORE_ADDR)1 << 63)
	| ((CORE_ADDR)fd << 32) | (CORE_ADDR) (pc - 4);
    }
  else if (register_size (regcache->tdesc, 0) == 4)
    {
      unsigned int pc;
      collect_register_by_name (regcache, "pc", &pc);
      return (CORE_ADDR) pc;
    }
  else
    {
      unsigned long pc;
      collect_register_by_name (regcache, "pc", &pc);
      return (CORE_ADDR) pc;
    }
}

static void
ppc_set_pc (struct regcache *regcache, CORE_ADDR pc)
{
  CORE_ADDR addr;
  int fd;

  if (parse_spufs_run (regcache, &fd, &addr))
    {
      unsigned int newpc = pc;
      (*the_target->write_memory) (addr, (unsigned char *) &newpc, 4);
    }
  else if (register_size (regcache->tdesc, 0) == 4)
    {
      unsigned int newpc = pc;
      supply_register_by_name (regcache, "pc", &newpc);
    }
  else
    {
      unsigned long newpc = pc;
      supply_register_by_name (regcache, "pc", &newpc);
    }
}


static int
ppc_get_hwcap (unsigned long *valp)
{
  const struct target_desc *tdesc = current_process ()->tdesc;
  int wordsize = register_size (tdesc, 0);
  unsigned char *data = (unsigned char *) alloca (2 * wordsize);
  int offset = 0;

  while ((*the_target->read_auxv) (offset, data, 2 * wordsize) == 2 * wordsize)
    {
      if (wordsize == 4)
	{
	  unsigned int *data_p = (unsigned int *)data;
	  if (data_p[0] == AT_HWCAP)
	    {
	      *valp = data_p[1];
	      return 1;
	    }
	}
      else
	{
	  unsigned long *data_p = (unsigned long *)data;
	  if (data_p[0] == AT_HWCAP)
	    {
	      *valp = data_p[1];
	      return 1;
	    }
	}

      offset += 2 * wordsize;
    }

  *valp = 0;
  return 0;
}

#ifndef __powerpc64__
static int ppc_regmap_adjusted;
#endif


/* Correct in either endianness.
   This instruction is "twge r2, r2", which GDB uses as a software
   breakpoint.  */
static const unsigned int ppc_breakpoint = 0x7d821008;
#define ppc_breakpoint_len 4

/* Implementation of linux_target_ops method "sw_breakpoint_from_kind".  */

static const gdb_byte *
ppc_sw_breakpoint_from_kind (int kind, int *size)
{
  *size = ppc_breakpoint_len;
  return (const gdb_byte *) &ppc_breakpoint;
}

static int
ppc_breakpoint_at (CORE_ADDR where)
{
  unsigned int insn;

  if (where & ((CORE_ADDR)1 << 63))
    {
      char mem_annex[32];
      sprintf (mem_annex, "%d/mem", (int)((where >> 32) & 0x7fffffff));
      (*the_target->qxfer_spu) (mem_annex, (unsigned char *) &insn,
				NULL, where & 0xffffffff, 4);
      if (insn == 0x3fff)
	return 1;
    }
  else
    {
      (*the_target->read_memory) (where, (unsigned char *) &insn, 4);
      if (insn == ppc_breakpoint)
	return 1;
      /* If necessary, recognize more trap instructions here.  GDB only uses
	 the one.  */
    }

  return 0;
}

/* Provide only a fill function for the general register set.  ps_lgetregs
   will use this for NPTL support.  */

static void ppc_fill_gregset (struct regcache *regcache, void *buf)
{
  int i;

  for (i = 0; i < 32; i++)
    ppc_collect_ptrace_register (regcache, i, (char *) buf + ppc_regmap[i]);

  for (i = 64; i < 70; i++)
    ppc_collect_ptrace_register (regcache, i, (char *) buf + ppc_regmap[i]);

  for (i = 71; i < 73; i++)
    ppc_collect_ptrace_register (regcache, i, (char *) buf + ppc_regmap[i]);
}

#define SIZEOF_VSXREGS 32*8

static void
ppc_fill_vsxregset (struct regcache *regcache, void *buf)
{
  int i, base;
  char *regset = (char *) buf;

  if (!(ppc_hwcap & PPC_FEATURE_HAS_VSX))
    return;

  base = find_regno (regcache->tdesc, "vs0h");
  for (i = 0; i < 32; i++)
    collect_register (regcache, base + i, &regset[i * 8]);
}

static void
ppc_store_vsxregset (struct regcache *regcache, const void *buf)
{
  int i, base;
  const char *regset = (const char *) buf;

  if (!(ppc_hwcap & PPC_FEATURE_HAS_VSX))
    return;

  base = find_regno (regcache->tdesc, "vs0h");
  for (i = 0; i < 32; i++)
    supply_register (regcache, base + i, &regset[i * 8]);
}

#define SIZEOF_VRREGS 33*16+4

static void
ppc_fill_vrregset (struct regcache *regcache, void *buf)
{
  int i, base;
  char *regset = (char *) buf;

  if (!(ppc_hwcap & PPC_FEATURE_HAS_ALTIVEC))
    return;

  base = find_regno (regcache->tdesc, "vr0");
  for (i = 0; i < 32; i++)
    collect_register (regcache, base + i, &regset[i * 16]);

  collect_register_by_name (regcache, "vscr", &regset[32 * 16 + 12]);
  collect_register_by_name (regcache, "vrsave", &regset[33 * 16]);
}

static void
ppc_store_vrregset (struct regcache *regcache, const void *buf)
{
  int i, base;
  const char *regset = (const char *) buf;

  if (!(ppc_hwcap & PPC_FEATURE_HAS_ALTIVEC))
    return;

  base = find_regno (regcache->tdesc, "vr0");
  for (i = 0; i < 32; i++)
    supply_register (regcache, base + i, &regset[i * 16]);

  supply_register_by_name (regcache, "vscr", &regset[32 * 16 + 12]);
  supply_register_by_name (regcache, "vrsave", &regset[33 * 16]);
}

struct gdb_evrregset_t
{
  unsigned long evr[32];
  unsigned long long acc;
  unsigned long spefscr;
};

static void
ppc_fill_evrregset (struct regcache *regcache, void *buf)
{
  int i, ev0;
  struct gdb_evrregset_t *regset = (struct gdb_evrregset_t *) buf;

  if (!(ppc_hwcap & PPC_FEATURE_HAS_SPE))
    return;

  ev0 = find_regno (regcache->tdesc, "ev0h");
  for (i = 0; i < 32; i++)
    collect_register (regcache, ev0 + i, &regset->evr[i]);

  collect_register_by_name (regcache, "acc", &regset->acc);
  collect_register_by_name (regcache, "spefscr", &regset->spefscr);
}

static void
ppc_store_evrregset (struct regcache *regcache, const void *buf)
{
  int i, ev0;
  const struct gdb_evrregset_t *regset = (const struct gdb_evrregset_t *) buf;

  if (!(ppc_hwcap & PPC_FEATURE_HAS_SPE))
    return;

  ev0 = find_regno (regcache->tdesc, "ev0h");
  for (i = 0; i < 32; i++)
    supply_register (regcache, ev0 + i, &regset->evr[i]);

  supply_register_by_name (regcache, "acc", &regset->acc);
  supply_register_by_name (regcache, "spefscr", &regset->spefscr);
}

/* Support for hardware single step.  */

static int
ppc_supports_hardware_single_step (void)
{
  return 1;
}

static struct regset_info ppc_regsets[] = {
  /* List the extra register sets before GENERAL_REGS.  That way we will
     fetch them every time, but still fall back to PTRACE_PEEKUSER for the
     general registers.  Some kernels support these, but not the newer
     PPC_PTRACE_GETREGS.  */
  { PTRACE_GETVSXREGS, PTRACE_SETVSXREGS, 0, SIZEOF_VSXREGS, EXTENDED_REGS,
  ppc_fill_vsxregset, ppc_store_vsxregset },
  { PTRACE_GETVRREGS, PTRACE_SETVRREGS, 0, SIZEOF_VRREGS, EXTENDED_REGS,
    ppc_fill_vrregset, ppc_store_vrregset },
  { PTRACE_GETEVRREGS, PTRACE_SETEVRREGS, 0, 32 * 4 + 8 + 4, EXTENDED_REGS,
    ppc_fill_evrregset, ppc_store_evrregset },
  { 0, 0, 0, 0, GENERAL_REGS, ppc_fill_gregset, NULL },
  NULL_REGSET
};

static struct usrregs_info ppc_usrregs_info =
  {
    ppc_num_regs,
    ppc_regmap,
  };

static struct regsets_info ppc_regsets_info =
  {
    ppc_regsets, /* regsets */
    0, /* num_regsets */
    NULL, /* disabled_regsets */
  };

static struct regs_info regs_info =
  {
    NULL, /* regset_bitmap */
    &ppc_usrregs_info,
    &ppc_regsets_info
  };

static const struct regs_info *
ppc_regs_info (void)
{
  return &regs_info;
}

static void
ppc_arch_setup (void)
{
  const struct target_desc *tdesc;
#ifdef __powerpc64__
  long msr;
  struct regcache *regcache;

  /* On a 64-bit host, assume 64-bit inferior process with no
     AltiVec registers.  Reset ppc_hwcap to ensure that the
     collect_register call below does not fail.  */
  tdesc = tdesc_powerpc_64l;
  current_process ()->tdesc = tdesc;
  ppc_hwcap = 0;

  regcache = new_register_cache (tdesc);
  fetch_inferior_registers (regcache, find_regno (tdesc, "msr"));
  collect_register_by_name (regcache, "msr", &msr);
  free_register_cache (regcache);
  if (ppc64_64bit_inferior_p (msr))
    {
      ppc_get_hwcap (&ppc_hwcap);
      if (ppc_hwcap & PPC_FEATURE_CELL)
	tdesc = tdesc_powerpc_cell64l;
      else if (ppc_hwcap & PPC_FEATURE_HAS_VSX)
	{
	  /* Power ISA 2.05 (implemented by Power 6 and newer processors)
	     increases the FPSCR from 32 bits to 64 bits. Even though Power 7
	     supports this ISA version, it doesn't have PPC_FEATURE_ARCH_2_05
	     set, only PPC_FEATURE_ARCH_2_06.  Since for now the only bits
	     used in the higher half of the register are for Decimal Floating
	     Point, we check if that feature is available to decide the size
	     of the FPSCR.  */
	  if (ppc_hwcap & PPC_FEATURE_HAS_DFP)
	    tdesc = tdesc_powerpc_isa205_vsx64l;
	  else
	    tdesc = tdesc_powerpc_vsx64l;
	}
      else if (ppc_hwcap & PPC_FEATURE_HAS_ALTIVEC)
	{
	  if (ppc_hwcap & PPC_FEATURE_HAS_DFP)
	    tdesc = tdesc_powerpc_isa205_altivec64l;
	  else
	    tdesc = tdesc_powerpc_altivec64l;
	}

      current_process ()->tdesc = tdesc;
      return;
    }
#endif

  /* OK, we have a 32-bit inferior.  */
  tdesc = tdesc_powerpc_32l;
  current_process ()->tdesc = tdesc;

  ppc_get_hwcap (&ppc_hwcap);
  if (ppc_hwcap & PPC_FEATURE_CELL)
    tdesc = tdesc_powerpc_cell32l;
  else if (ppc_hwcap & PPC_FEATURE_HAS_VSX)
    {
      if (ppc_hwcap & PPC_FEATURE_HAS_DFP)
	tdesc = tdesc_powerpc_isa205_vsx32l;
      else
	tdesc = tdesc_powerpc_vsx32l;
    }
  else if (ppc_hwcap & PPC_FEATURE_HAS_ALTIVEC)
    {
      if (ppc_hwcap & PPC_FEATURE_HAS_DFP)
	tdesc = tdesc_powerpc_isa205_altivec32l;
      else
	tdesc = tdesc_powerpc_altivec32l;
    }

  /* On 32-bit machines, check for SPE registers.
     Set the low target's regmap field as appropriately.  */
#ifndef __powerpc64__
  if (ppc_hwcap & PPC_FEATURE_HAS_SPE)
    tdesc = tdesc_powerpc_e500l;

  if (!ppc_regmap_adjusted)
    {
      if (ppc_hwcap & PPC_FEATURE_HAS_SPE)
	ppc_usrregs_info.regmap = ppc_regmap_e500;

      /* If the FPSCR is 64-bit wide, we need to fetch the whole
	 64-bit slot and not just its second word.  The PT_FPSCR
	 supplied in a 32-bit GDB compilation doesn't reflect
	 this.  */
      if (register_size (tdesc, 70) == 8)
	ppc_regmap[70] = (48 + 2*32) * sizeof (long);

      ppc_regmap_adjusted = 1;
   }
#endif
  current_process ()->tdesc = tdesc;
}

struct linux_target_ops the_low_target = {
  ppc_arch_setup,
  ppc_regs_info,
  ppc_cannot_fetch_register,
  ppc_cannot_store_register,
  NULL, /* fetch_register */
  ppc_get_pc,
  ppc_set_pc,
  NULL, /* breakpoint_kind_from_pc */
  ppc_sw_breakpoint_from_kind,
  NULL,
  0,
  ppc_breakpoint_at,
  NULL, /* supports_z_point_type */
  NULL,
  NULL,
  NULL,
  NULL,
  ppc_collect_ptrace_register,
  ppc_supply_ptrace_register,
  NULL, /* siginfo_fixup */
  NULL, /* new_process */
  NULL, /* new_thread */
  NULL, /* new_fork */
  NULL, /* prepare_to_resume */
  NULL, /* process_qsupported */
  NULL, /* supports_tracepoints */
  NULL, /* get_thread_area */
  NULL, /* install_fast_tracepoint_jump_pad */
  NULL, /* emit_ops */
  NULL, /* get_min_fast_tracepoint_insn_len */
  NULL, /* supports_range_stepping */
  NULL, /* breakpoint_kind_from_current_state */
  ppc_supports_hardware_single_step,
};

void
initialize_low_arch (void)
{
  /* Initialize the Linux target descriptions.  */

  init_registers_powerpc_32l ();
  init_registers_powerpc_altivec32l ();
  init_registers_powerpc_cell32l ();
  init_registers_powerpc_vsx32l ();
  init_registers_powerpc_isa205_32l ();
  init_registers_powerpc_isa205_altivec32l ();
  init_registers_powerpc_isa205_vsx32l ();
  init_registers_powerpc_e500l ();
  init_registers_powerpc_64l ();
  init_registers_powerpc_altivec64l ();
  init_registers_powerpc_cell64l ();
  init_registers_powerpc_vsx64l ();
  init_registers_powerpc_isa205_64l ();
  init_registers_powerpc_isa205_altivec64l ();
  init_registers_powerpc_isa205_vsx64l ();

  initialize_regsets_info (&ppc_regsets_info);
}
