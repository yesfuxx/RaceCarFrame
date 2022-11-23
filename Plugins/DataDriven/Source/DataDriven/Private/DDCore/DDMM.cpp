// Fill out your copyright notice in the Description page of Project Settings.


#include "DDCore/DDMM.h"
#include "DDCore/DDModule.h"
#include "DDCore/DDDriver.h"

// Add default functionality here for any IDDMM functions that are not pure virtual.
void IDDMM::AssignModule(UDDModule* Mod)
{
	IModule = Mod;
	ModuleIndex = IModule->ModuleIndex;
	IDriver = UDDCommon::Get()->GetDriver();
}

void IDDMM::ExecuteFunction(DDModuleAgreement Agreement, DDParam* Param)
{
	if (Agreement.ModuleIndex == ModuleIndex)
		IModule->ExecuteFunction(Agreement, Param);
	else
		IDriver->ExecuteFunction(Agreement, Param);	
}

void IDDMM::ExecuteFunction(DDObjectAgreement Agreement, DDParam* Param)
{
	if (Agreement.ModuleIndex == ModuleIndex)
		IModule->ExecuteFunction(Agreement, Param);
	else
		IDriver->ExecuteFunction(Agreement, Param);
}
