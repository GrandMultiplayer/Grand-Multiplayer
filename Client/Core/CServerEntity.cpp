


void CServerEntity::Revive()
{
	switch (Data.type)
	{
	case CServerEntity::Player:
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			if (g_Players[i].GetId() == Data.Id)
			{
				g_Players[i].Revive();
				return;
			}
		}
		break;
	case CServerEntity::NPC:
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			if (g_Npcs[i].GetId() == Data.Id)
			{
				g_Npcs[i].Revive();
				return;
			}
		}
		break;
	default:
		LOG_DEBUG("[CServerEntity::Revive] Invalid entity.");
		return;
		break;
	}
}

