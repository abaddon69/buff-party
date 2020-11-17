//find:
	if (dwVnum == SKILL_CHAIN)
	{
		ResetChainLightningIndex();
		AddChainLightningExcept(pkVictim);
	}
	
//add after:
	if (GetParty() && (dwVnum == 94 || dwVnum == 95 || dwVnum == 96 || dwVnum == 109 || dwVnum == 110 || dwVnum == 111))
	{
		if (pkVictim && pkVictim->GetParty())
		{
			if (pkVictim->GetParty() == GetParty())
			{
				if (GetParty()->GetNearMemberCount())
					ComputeSkillParty(dwVnum, this);
				else
					ComputeSkill(dwVnum, pkVictim);
			}
			else
				ComputeSkill(dwVnum, pkVictim);
		}
		else if (pkVictim && !pkVictim->GetParty())
		{
			ComputeSkill(dwVnum, pkVictim);
		}
	}

//find:
int CHARACTER::ComputeSkill(DWORD dwVnum, LPCHARACTER pkVictim, BYTE bSkillLevel)

//add before:
struct FComputeSkillParty
{
	FComputeSkillParty(DWORD dwVnum, LPCHARACTER pkAttacker, BYTE bSkillLevel = 0)
		: m_dwVnum(dwVnum), m_pkAttacker(pkAttacker), m_bSkillLevel(bSkillLevel)
		{
		}

	void operator () (LPCHARACTER ch)
	{
		m_pkAttacker->ComputeSkill(m_dwVnum, ch, m_bSkillLevel);
	}

	DWORD m_dwVnum;
	LPCHARACTER m_pkAttacker;
	BYTE m_bSkillLevel;
};

int CHARACTER::ComputeSkillParty(DWORD dwVnum, LPCHARACTER pkVictim, BYTE bSkillLevel)
{
	FComputeSkillParty f(dwVnum, pkVictim, bSkillLevel);
	if (GetParty() && GetParty()->GetNearMemberCount())
		GetParty()->ForEachNearMember(f);
	else
		f(this);

	return BATTLE_NONE;
}