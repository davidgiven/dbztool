#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "globals.h"
#include "mc68328.h"
#include <map>
#include <fnmatch.h>

static const std::map<std::string, Register> registers =
{
	{ "SCR", 	  { SCR, 1 }},
	{ "PCR", 	  { PCR, 1 }},
	{ "IDR", 	  { IDR, 4 }},
	{ "IODCR",    { IODCR, 2 }},
	{ "CSGBA", 	  { CSGBA, 2 }},
	{ "CSGBB", 	  { CSGBB, 2 }},
	{ "CSGBC", 	  { CSGBC, 2 }},
	{ "CSGBD", 	  { CSGBD, 2 }},
	{ "CSUGBA",   { CSUGBA, 2 }},
	{ "CSCTRL1",  { CSCTRL1, 2 }},
	{ "CSCTRL2",  { CSCTRL2, 2 }},
	{ "CSA",      { CSA, 2 }},
	{ "CSB",      { CSB, 2 }},
	{ "CSC",      { CSC, 2 }},
	{ "CSD",      { CSD, 2 }},
	{ "EMUCS",    { EMUCS, 2 }},
	{ "PLLCR",    { PLLCR, 2 }},
	{ "PLLFSR",   { PLLFSR, 2 }},
	{ "PCTLR",    { PCTLR, 1 }},
	{ "IVR",      { IVR, 1 }},
	{ "ICR",      { ICR, 2 }},
	{ "IMR",      { IMR, 4 }},
	{ "ISR",      { ISR, 4 }},
	{ "IPR",      { IPR, 4 }},
	{ "ILCR",     { ILCR, 2 }},
	{ "PADIR",    { PADIR, 1 }},
	{ "PADATA",   { PADATA, 1 }},
	{ "PAPUEN",   { PAPUEN, 1 }},
	{ "PBDIR",    { PBDIR, 1 }},
	{ "PBDATA",   { PBDATA, 1 }},
	{ "PBPUEN",   { PBPUEN, 1 }},
	{ "PBSEL",    { PBSEL, 1 }},
	{ "PCDIR",    { PCDIR, 1 }},
	{ "PCDATA",   { PCDATA, 1 }},
	{ "PCPDEN",   { PCPDEN, 1 }},
	{ "PCSEL",    { PCSEL, 1 }},
	{ "PDDIR",    { PDDIR, 1 }},
	{ "PDDATA",   { PDDATA, 1 }},
	{ "PDPUEN",   { PDPUEN, 1 }},
	{ "PDSEL",    { PDSEL, 1 }},
	{ "PDPOL",    { PDPOL, 1 }},
	{ "PDIRQEN",  { PDIRQEN, 1 }},
	{ "PDKBEN",   { PDKBEN, 1 }},
	{ "PDIRQEG",  { PDIRQEG, 1 }},
	{ "PEDIR",    { PEDIR, 1 }},
	{ "PEDATA",   { PEDATA, 1 }},
	{ "PEPUEN",   { PEPUEN, 1 }},
	{ "PESEL",    { PESEL, 1 }},
	{ "PFDIR",    { PFDIR, 1 }},
	{ "PFDATA",   { PFDATA, 1 }},
	{ "PFPUEN",   { PFPUEN, 1 }},
	{ "PFSEL",    { PFSEL, 1 }},
	{ "PGDIR",    { PGDIR, 1 }},
	{ "PGDATA",   { PGDATA, 1 }},
	{ "PGPUEN",   { PGPUEN, 1 }},
	{ "PGSEL",    { PGSEL, 1 }},
	{ "PJDIR",    { PJDIR, 1 }},
	{ "PJDATA",   { PJDATA, 1 }},
	{ "PJPUEN",   { PJPUEN, 1 }},
	{ "PJSEL",    { PJSEL, 1 }},
	{ "PKDIR",    { PKDIR, 1 }},
	{ "PKDATA",   { PKDATA, 1 }},
	{ "PKPUEN",   { PKPUEN, 1 }},
	{ "PKSEL",    { PKSEL, 1 }},
	{ "PMDIR",    { PMDIR, 1 }},
	{ "PMDATA",   { PMDATA, 1 }},
	{ "PMPUEN",   { PMPUEN, 1 }},
	{ "PMSEL",    { PMSEL, 1 }},
	{ "PWMC1",    { PWMC1, 2 }},
	{ "PWMS1",    { PWMS1, 2 }},
	{ "PWMP1",    { PWMP1, 1 }},
	{ "PCMCNT1",  { PCMCNT1, 1 }},
	{ "PWMC2",    { PWMC2, 2 }},
	{ "PWMS2",    { PWMS2, 2 }},
	{ "PWMP2",    { PWMP2, 2 }},
	{ "PCMCNT2",  { PCMCNT2, 2 }},
	{ "TCTL1",    { TCTL1, 2 }},
	{ "TPRER1",   { TPRER1, 2 }},
	{ "TCMP1",    { TCMP1, 2 }},
	{ "TCR1",     { TCR1, 2 }},
	{ "TCN1",     { TCN1, 2 }},
	{ "TSTAT1",   { TSTAT1, 2 }},
	{ "TCTL2",    { TCTL2, 2 }},
	{ "TPRER2",   { TPRER2, 2 }},
	{ "TCMP2",    { TCMP2, 2 }},
	{ "TCR2",     { TCR2, 2 }},
	{ "TCN2",     { TCN2, 2 }},
	{ "TSTAT2",   { TSTAT2, 2 }},
	{ "SPIRXD",   { SPIRXD, 2 }},
	{ "SPITXD",   { SPITXD, 2 }},
	{ "SPICONT1", { SPICONT1, 2 }},
	{ "SPIINTCS", { SPIINTCS, 2 }},
	{ "SPITEST",  { SPITEST, 2 }},
	{ "SPISPC",   { SPISPC, 2 }},
	{ "SPIDATA2", { SPIDATA2, 2 }},
	{ "SPICONT2", { SPICONT2, 2 }},
	{ "USTCNT1",  { USTCNT1, 2 }},
	{ "UBAUD1",   { UBAUD1, 2 }},
	{ "URX1",     { URX1, 2 }},
	{ "UTX1",     { UTX1, 2 }},
	{ "UMISC1",   { UMISC1, 2 }},
	{ "NIPR1",    { NIPR1, 2 }},
	{ "USTCNT2",  { USTCNT2, 2 }},
	{ "UBAUD2",   { UBAUD2, 2 }},
	{ "URX2",     { URX2, 2 }},
	{ "UTX2",     { UTX2, 2 }},
	{ "UMISC2",   { UMISC2, 2 }},
	{ "NIPR2",    { NIPR2, 2 }},
	{ "HMARK",    { HMARK, 2 }},
	{ "LSSA",     { LSSA, 4 }},
	{ "LVPW",     { LVPW, 1 }},
	{ "LXMAX",    { LXMAX, 2 }},
	{ "LYMAX",    { LYMAX, 2 }},
	{ "LCXP",     { LCXP, 2 }},
	{ "LXYP",     { LXYP, 2 }},
	{ "LCWCH",    { LCWCH, 2 }},
	{ "LBLKC",    { LBLKC, 1 }},
	{ "LPICF",    { LPICF, 1 }},
	{ "LPOLCF",   { LPOLCF, 1 }},
	{ "LACDRC",   { LACDRC, 1 }},
	{ "LPXCD",    { LPXCD, 1 }},
	{ "LCKCON",   { LCKCON, 1 }},
	{ "LRRA",     { LRRA, 1 }},
	{ "LPOSR",    { LPOSR, 1 }},
	{ "LFRCM",    { LFRCM, 1 }},
	{ "LGPMR",    { LGPMR, 1 }},
	{ "PWMR",     { PWMR, 2 }},
	{ "RMCR",     { RMCR, 1 }},
	{ "DMACR",    { DMACR, 1 }},
	{ "RTCTIME",  { RTCTIME, 4 }},
	{ "RTCALRM",  { RTCALRM, 4 }},
	{ "WATCHDOG", { WATCHDOG, 2 }},
	{ "RTCCTL",   { RTCCTL, 1 }},
	{ "RTCISR",   { RTCISR, 2 }},
	{ "RTCIENR",  { RTCIENR, 2 }},
	{ "STPWCH",   { STPWCH, 1 }},
	{ "DAYR",     { DAYR, 2 }},
	{ "DAYALARM", { DAYALARM, 2 }},
	{ "DRAMMC",   { DRAMMC, 2 }},
	{ "DRAMC",    { DRAMC, 2 }},
	{ "SDCTRL",   { SDCTRL, 2 }},
	{ "SDPWPN",   { SDPWPN, 2 }},
	{ "ICEMACR",  { ICEMACR, 4 }},
	{ "ICEMAMR",  { ICEMAMR, 4 }},
	{ "ICEMCCR",  { ICEMCCR, 2 }},
	{ "ICEMCMR",  { ICEMCMR, 2 }},
	{ "ICEMCR",   { ICEMCR, 2 }},
	{ "ICEMSR",   { ICEMSR, 2 }},
};

const Register* find_register(const char* name)
{
	const auto& i = registers.find(name);
	if (i == registers.end())
		return nullptr;

	return &i->second;
}

bool foreach_register(const char* glob, std::function<bool(const std::string& name, const Register&)> cb)
{
	for (const auto& i : registers)
	{
		if (fnmatch(glob, i.first.c_str(), FNM_CASEFOLD|FNM_EXTMATCH) == 0)
		{
			if (cb(i.first, i.second))
				return true;
		}
	}
	return false;
}

