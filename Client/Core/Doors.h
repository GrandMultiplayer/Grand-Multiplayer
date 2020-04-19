#ifndef __DOORS_H__
#define __DOORS_H__
#include "hook/natives.h"

#include "Functions.h"
#include "CVector3.h"

class Doors
{
public:
	static void Pulse();

	static const int MAXDOORS = 225;

	typedef struct
	{
		Hash modelInfoHash;	//doorType
		CVector3 coords;	//doorCords
		int doorState;		//doorState
		int doorHash;		//doorHash/Model
		float heading;		//doorOpenState
	} DoorsInfo;
};

static inline Doors::DoorsInfo GetDoorInfo(int doorIndex)
{
	Doors::DoorsInfo doorInfo;
	switch (doorIndex)
	{
	case 0:
		doorInfo.modelInfoHash = $("v_ilev_bs_door");
		doorInfo.coords = { 133.0f, -1711.0f, 29.0f };
		doorInfo.doorHash = 0x6b918ea1;
		break;
	case 1:
		doorInfo.modelInfoHash = $("v_ilev_bs_door");
		doorInfo.coords = { -1287.857f, -1115.742f, 7.1401f };
		doorInfo.doorHash = 0x53a940ab;
		break;
	case 2:
		doorInfo.modelInfoHash = $("v_ilev_bs_door");
		doorInfo.coords = { 1932.952f, 3725.154f, 32.9944f };
		doorInfo.doorHash = 0x86ef4558;
		break;
	case 3:
		doorInfo.modelInfoHash = $("v_ilev_bs_door");
		doorInfo.coords = { 1207.873f, -470.063f, 66.358f };
		doorInfo.doorHash = 1796834809;
		break;

	case 4:
		doorInfo.modelInfoHash = $("v_ilev_bs_door");
		doorInfo.coords = { -29.8692f, -148.1571f, 57.2265f };
		doorInfo.doorHash = 96153298;
		break;

	case 5:
		doorInfo.modelInfoHash = $("v_ilev_bs_door");
		doorInfo.coords = { -280.7851f, 6232.782f, 31.8455f };
		doorInfo.doorHash = -281080954;
		break;

	case 6:
		doorInfo.modelInfoHash = $("v_ilev_hd_door_l");
		doorInfo.coords = { -824.0f, -187.0f, 38.0f };
		doorInfo.doorHash = 183249434;
		break;

	case 7:
		doorInfo.modelInfoHash = $("v_ilev_hd_door_r");
		doorInfo.coords = { -823.0f, -188.0f, 38.0f };
		doorInfo.doorHash = 758345384;
		break;

	case 8:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01");
		doorInfo.coords = { 82.3186f, -1392.752f, 29.5261f };
		doorInfo.doorHash = -1069262641;
		break;

	case 9:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01_r");
		doorInfo.coords = { 82.3186f, -1390.476f, 29.5261f };
		doorInfo.doorHash = 1968521986;
		break;

	case 10:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01");
		doorInfo.coords = { 1686.983f, 4821.741f, 42.2131f };
		doorInfo.doorHash = -2143706301;
		break;

	case 11:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01_r");
		doorInfo.coords = { 1687.282f, 4819.484f, 42.2131f };
		doorInfo.doorHash = -1403421822;
		break;

	case 12:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01");
		doorInfo.coords = { 418.637f, -806.457f, 29.6396f };
		doorInfo.doorHash = -1950137670;
		break;

	case 13:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01_r");
		doorInfo.coords = { 418.637f, -808.733f, 29.6396f };
		doorInfo.doorHash = 1226259807;
		break;

	case 14:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01");
		doorInfo.coords = { -1096.661f, 2705.446f, 19.2578f };
		doorInfo.doorHash = 1090833557;
		break;

	case 15:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01_r");
		doorInfo.coords = { -1094.965f, 2706.964f, 19.2578f };
		doorInfo.doorHash = 897332612;
		break;

	case 16:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01");
		doorInfo.coords = { 1196.825f, 2703.221f, 38.3726f };
		doorInfo.doorHash = 1095946640;
		break;

	case 17:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01_r");
		doorInfo.coords = { 1199.101f, 2703.221f, 38.3726f };
		doorInfo.doorHash = 801975945;
		break;

	case 18:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01");
		doorInfo.coords = { -818.7642f, -1079.544f, 11.4781f };
		doorInfo.doorHash = -167996547;
		break;

	case 19:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01_r");
		doorInfo.coords = { -816.7932f, -1078.406f, 11.4781f };
		doorInfo.doorHash = -1935818563;
		break;

	case 20:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01");
		doorInfo.coords = { -0.0564f, 6517.461f, 32.0278f };
		doorInfo.doorHash = 1891185217;
		break;

	case 21:
		doorInfo.modelInfoHash = $("v_ilev_cs_door01_r");
		doorInfo.coords = { -1.7253f, 6515.914f, 32.0278f };
		doorInfo.doorHash = 1236591681;
		break;

	case 22:
		doorInfo.modelInfoHash = $("v_ilev_clothmiddoor");
		doorInfo.coords = { -1201.435f, -776.8566f, 17.9918f };
		doorInfo.doorHash = 1980808685;
		break;

	case 23:
		doorInfo.modelInfoHash = $("v_ilev_clothmiddoor");
		doorInfo.coords = { 617.2458f, 2751.022f, 42.7578f };
		doorInfo.doorHash = 1352749757;
		break;

	case 24:
		doorInfo.modelInfoHash = $("v_ilev_clothmiddoor");
		doorInfo.coords = { 127.8201f, -211.8274f, 55.2275f };
		doorInfo.doorHash = -566554453;
		break;

	case 25:
		doorInfo.modelInfoHash = $("v_ilev_clothmiddoor");
		doorInfo.coords = { -3167.75f, 1055.536f, 21.5329f };
		doorInfo.doorHash = 1284749450;
		break;

	case 26:
		doorInfo.modelInfoHash = $("v_ilev_ch_glassdoor");
		doorInfo.coords = { -716.6754f, -155.42f, 37.6749f };
		doorInfo.doorHash = 261851994;
		break;

	case 27:
		doorInfo.modelInfoHash = $("v_ilev_ch_glassdoor");
		doorInfo.coords = { -715.6154f, -157.2561f, 37.6749f };
		doorInfo.doorHash = 217646625;
		break;

	case 28:
		doorInfo.modelInfoHash = $("v_ilev_ch_glassdoor");
		doorInfo.coords = { -157.0924f, -306.4413f, 39.994f };
		doorInfo.doorHash = 1801139578;
		break;

	case 29:
		doorInfo.modelInfoHash = $("v_ilev_ch_glassdoor");
		doorInfo.coords = { -156.4022f, -304.4366f, 39.994f };
		doorInfo.doorHash = -2123275866;
		break;

	case 30:
		doorInfo.modelInfoHash = $("v_ilev_ch_glassdoor");
		doorInfo.coords = { -1454.782f, -231.7927f, 50.0565f };
		doorInfo.doorHash = 1312689981;
		break;

	case 31:
		doorInfo.modelInfoHash = $("v_ilev_ch_glassdoor");
		doorInfo.coords = { -1456.201f, -233.3682f, 50.0565f };
		doorInfo.doorHash = -595055661;
		break;

	case 32:
		doorInfo.modelInfoHash = $("v_ilev_ta_door");
		doorInfo.coords = { 321.81f, 178.36f, 103.68f };
		doorInfo.doorHash = -265260897;
		break;

	case 33:
		doorInfo.modelInfoHash = -1212951353;
		doorInfo.coords = { 1859.89f, 3749.79f, 33.18f };
		doorInfo.doorHash = -1284867488;
		break;

	case 34:
		doorInfo.modelInfoHash = -1212951353;
		doorInfo.coords = { -289.1752f, 6199.112f, 31.637f };
		doorInfo.doorHash = 302307081;
		break;

	case 35:
		doorInfo.modelInfoHash = $("v_ilev_ta_door");
		doorInfo.coords = { -1155.454f, -1424.008f, 5.0461f };
		doorInfo.doorHash = -681886015;
		break;

	case 36:
		doorInfo.modelInfoHash = $("v_ilev_ta_door");
		doorInfo.coords = { 1321.286f, -1650.597f, 52.3663f };
		doorInfo.doorHash = -2086556500;
		break;

	case 37:
		doorInfo.modelInfoHash = $("v_ilev_ta_door");
		doorInfo.coords = { -3167.789f, 1074.767f, 20.9209f };
		doorInfo.doorHash = -1496386696;
		break;

	case 38:
		doorInfo.modelInfoHash = $("v_ilev_mm_doorm_l");
		doorInfo.coords = { -817.0f, 179.0f, 73.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = -2097039789;
		break;

	case 39:
		doorInfo.modelInfoHash = $("v_ilev_mm_doorm_r");
		doorInfo.coords = { -816.0f, 178.0f, 73.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = -2127416656;
		break;

	case 40:
		doorInfo.modelInfoHash = $("prop_ld_garaged_01");
		doorInfo.coords = { -815.0f, 186.0f, 73.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = -1986583853;
		doorInfo.heading = 6.5f;
		break;

	case 41:
		doorInfo.modelInfoHash = $("prop_bh1_48_backdoor_l");
		doorInfo.coords = { -797.0f, 177.0f, 73.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = 776026812;
		break;

	case 42:
		doorInfo.modelInfoHash = $("prop_bh1_48_backdoor_r");
		doorInfo.coords = { -795.0f, 178.0f, 73.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = 698422331;
		break;

	case 43:
		doorInfo.modelInfoHash = $("prop_bh1_48_backdoor_l");
		doorInfo.coords = { -793.0f, 181.0f, 73.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = 535076355;
		break;

	case 44:
		doorInfo.modelInfoHash = $("prop_bh1_48_backdoor_r");
		doorInfo.coords = { -794.0f, 183.0f, 73.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = 474675599;
		break;

	case 45:
		doorInfo.modelInfoHash = $("prop_bh1_48_gate_1");
		doorInfo.coords = { -849.0f, 179.0f, 70.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = -1978427516;
		break;

	case 46:
		doorInfo.modelInfoHash = $("v_ilev_mm_windowwc");
		doorInfo.coords = { -802.7333f, 167.5041f, 77.5824f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = -1700375831;
		break;

	case 47:
		doorInfo.modelInfoHash = $("v_ilev_fa_frontdoor");
		doorInfo.coords = { -14.0f, -1441.0f, 31.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = 613961892;
		break;

	case 48:
		doorInfo.modelInfoHash = $("v_ilev_fh_frntdoor");
		doorInfo.coords = { -15.0f, -1427.0f, 31.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = -272570634;
		break;

	case 49:
		doorInfo.modelInfoHash = $("prop_sc1_21_g_door_01");
		doorInfo.coords = { -25.28f, -1431.06f, 30.84f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = -1040675994;
		break;

	case 50:
		doorInfo.modelInfoHash = $("v_ilev_fh_frontdoor");
		doorInfo.coords = { 7.52f, 539.53f, 176.18f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = 1201219326;
		break;

	case 51:
		doorInfo.modelInfoHash = $("v_ilev_trevtraildr");
		doorInfo.coords = { 1973.0f, 3815.0f, 34.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = 1736361794;
		break;

	case 52:
		doorInfo.modelInfoHash = $("prop_cs4_10_tr_gd_01");
		doorInfo.coords = { 1972.787f, 3824.554f, 32.5831f };
		doorInfo.doorHash = 1113956670;
		doorInfo.heading = 12.0f;
		break;

	case 53:
		doorInfo.modelInfoHash = $("v_ilev_trev_doorfront");
		doorInfo.coords = { -1150.0f, -1521.0f, 11.0f };
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 0);
		doorInfo.doorHash = -1361617046;
		break;
	case 54:
		doorInfo.modelInfoHash = $("prop_com_ls_door_01");
		doorInfo.coords = { -1145.9f, -1991.14f, 14.18f };
		doorInfo.doorHash = -1871080926;
		doorInfo.heading = 25.0f;
		break;

	case 55:
		doorInfo.modelInfoHash = $("prop_id2_11_gdoor");
		doorInfo.coords = { 723.12f, -1088.83f, 23.28f };
		doorInfo.doorHash = 1168079979;
		doorInfo.heading = 25.0f;
		break;

	case 56:
		doorInfo.modelInfoHash = $("prop_com_ls_door_01");
		doorInfo.coords = { -356.09f, -134.77f, 40.01f };
		doorInfo.doorHash = 1206354175;
		doorInfo.heading = 25.0f;
		break;

	case 57:
		doorInfo.modelInfoHash = $("v_ilev_carmod3door");
		doorInfo.coords = { 108.8502f, 6617.876f, 32.673f };
		doorInfo.doorHash = -1038180727;
		doorInfo.heading = 25.0f;
		break;

	case 58:
		doorInfo.modelInfoHash = $("v_ilev_carmod3door");
		doorInfo.coords = { 114.3206f, 6623.226f, 32.7161f };
		doorInfo.doorHash = 1200466273;
		doorInfo.heading = 25.0f;
		break;

	case 59:
		doorInfo.modelInfoHash = $("v_ilev_carmod3door");
		doorInfo.coords = { 1182.305f, 2645.242f, 38.807f };
		doorInfo.doorHash = 1391004277;
		doorInfo.heading = 25.0f;
		break;

	case 60:
		doorInfo.modelInfoHash = $("v_ilev_carmod3door");
		doorInfo.coords = { 1174.654f, 2645.242f, 38.6826f };
		doorInfo.doorHash = -459199009;
		doorInfo.heading = 25.0f;
		break;

	case 61:
		doorInfo.modelInfoHash = $("v_ilev_janitor_frontdoor");
		doorInfo.coords = { -107.5401f, -9.0258f, 70.6696f };
		doorInfo.doorHash = -252283844;
		break;

	case 62:
		doorInfo.modelInfoHash = $("v_ilev_ss_door8");
		doorInfo.coords = { 717.0f, -975.0f, 25.0f };
		doorInfo.doorHash = -826072862;
		break;

	case 63:
		doorInfo.modelInfoHash = $("v_ilev_ss_door7");
		doorInfo.coords = { 719.0f, -975.0f, 25.0f };
		doorInfo.doorHash = 763780711;
		break;

	case 64:
		doorInfo.modelInfoHash = $("v_ilev_ss_door02");
		doorInfo.coords = { 709.9813f, -963.5311f, 30.5453f };
		doorInfo.doorHash = -874851305;
		break;

	case 65:
		doorInfo.modelInfoHash = $("v_ilev_ss_door03");
		doorInfo.coords = { 709.9894f, -960.6675f, 30.5453f };
		doorInfo.doorHash = -1480820165;
		break;

	case 66:
		doorInfo.modelInfoHash = $("v_ilev_store_door");
		doorInfo.coords = { 707.8046f, -962.4564f, 30.5453f };
		doorInfo.doorHash = 949391213;
		break;

	case 67:
		doorInfo.modelInfoHash = -1212951353;
		doorInfo.coords = { 1393.0f, 3599.0f, 35.0f };
		doorInfo.doorHash = 212192855;
		break;

	case 68:
		doorInfo.modelInfoHash = -1212951353;
		doorInfo.coords = { 1395.0f, 3600.0f, 35.0f };
		doorInfo.doorHash = -126474752;
		break;

	case 69:
		doorInfo.modelInfoHash = $("v_ilev_ss_door04");
		doorInfo.coords = { 1387.0f, 3614.0f, 39.0f };
		doorInfo.doorHash = 1765671336;
		break;

	case 70:
		doorInfo.modelInfoHash = $("prop_ron_door_01");
		doorInfo.coords = { 1083.547f, -1975.435f, 31.6222f };
		doorInfo.doorHash = 792295685;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 71:
		doorInfo.modelInfoHash = $("prop_ron_door_01");
		doorInfo.coords = { 1065.237f, -2006.079f, 32.2329f };
		doorInfo.doorHash = 563273144;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 72:
		doorInfo.modelInfoHash = $("prop_ron_door_01");
		doorInfo.coords = { 1085.307f, -2018.561f, 41.6289f };
		doorInfo.doorHash = -726993043;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 73:
		doorInfo.modelInfoHash = $("v_ilev_bank4door02");
		doorInfo.coords = { -111.0f, 6464.0f, 32.0f };
		doorInfo.doorHash = 178228075;
		break;

	case 74:
		doorInfo.modelInfoHash = $("v_ilev_bank4door01");
		doorInfo.coords = { -110.0f, 6462.0f, 32.0f };
		doorInfo.doorHash = 1852297978;
		break;

	case 75:
		doorInfo.modelInfoHash = $("v_ilev_lester_doorfront");
		doorInfo.coords = { 1274.0f, -1721.0f, 55.0f };
		doorInfo.doorHash = -565026078;
		break;

	case 76:
		doorInfo.modelInfoHash = $("v_ilev_lester_doorveranda");
		doorInfo.coords = { 1271.89f, -1707.57f, 53.79f };
		doorInfo.doorHash = 1646172266;
		break;

	case 77:
		doorInfo.modelInfoHash = $("v_ilev_lester_doorveranda");
		doorInfo.coords = { 1270.77f, -1708.1f, 53.75f };
		doorInfo.doorHash = 204467342;
		break;

	case 78:
		doorInfo.modelInfoHash = $("v_ilev_deviantfrontdoor");
		doorInfo.coords = { -127.5f, -1456.18f, 37.94f };
		doorInfo.doorHash = 2047070410;
		break;

	case 79:
		doorInfo.modelInfoHash = $("prop_com_gar_door_01");
		doorInfo.coords = { 483.56f, -1316.08f, 32.18f };
		doorInfo.doorHash = 1417775309;
		break;

	case 80:
		doorInfo.modelInfoHash = $("v_ilev_cs_door");
		doorInfo.coords = { 483.0f, -1312.0f, 29.0f };
		doorInfo.doorHash = -106474626;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 81:
		doorInfo.modelInfoHash = $("prop_strip_door_01");
		doorInfo.coords = { 128.0f, -1299.0f, 29.0f };
		doorInfo.doorHash = 1840510598;
		break;

	case 82:
		doorInfo.modelInfoHash = $("prop_magenta_door");
		doorInfo.coords = { 96.0f, -1285.0f, 29.0f };
		doorInfo.doorHash = 1382825971;
		break;

	case 83:
		doorInfo.modelInfoHash = $("prop_motel_door_09");
		doorInfo.coords = { 549.0f, -1773.0f, 34.0f };
		doorInfo.doorHash = 232536303;
		break;

	case 84:
		doorInfo.modelInfoHash = $("v_ilev_gangsafedoor");
		doorInfo.coords = { 974.0f, -1839.0f, 36.0f };
		doorInfo.doorHash = 1267246609;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 3);
		break;

	case 85:
		doorInfo.modelInfoHash = $("v_ilev_gangsafedoor");
		doorInfo.coords = { 977.0f, -105.0f, 75.0f };
		doorInfo.doorHash = -1900237971;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 3);
		break;

	case 86:
		doorInfo.modelInfoHash = $("v_ilev_ra_door1_l");
		doorInfo.coords = { 1391.0f, 1163.0f, 114.0f };
		doorInfo.doorHash = 2077901353;
		break;

	case 87:
		doorInfo.modelInfoHash = $("v_ilev_ra_door1_r");
		doorInfo.coords = { 1391.0f, 1161.0f, 114.0f };
		doorInfo.doorHash = -2102079126;
		break;

	case 88:
		doorInfo.modelInfoHash = $("prop_cs6_03_door_l");
		doorInfo.coords = { 1396.0f, 1143.0f, 115.0f };
		doorInfo.doorHash = -1905793212;
		break;

	case 89:
		doorInfo.modelInfoHash = $("prop_cs6_03_door_r");
		doorInfo.coords = { 1396.0f, 1141.0f, 115.0f };
		doorInfo.doorHash = -1797032505;
		break;

	case 90:
		doorInfo.modelInfoHash = $("v_ilev_ra_door1_l");
		doorInfo.coords = { 1409.0f, 1146.0f, 114.0f };
		doorInfo.doorHash = -62235167;
		break;

	case 91:
		doorInfo.modelInfoHash = $("v_ilev_ra_door1_r");
		doorInfo.coords = { 1409.0f, 1148.0f, 114.0f };
		doorInfo.doorHash = -1727188163;
		break;

	case 92:
		doorInfo.modelInfoHash = $("v_ilev_ra_door1_l");
		doorInfo.coords = { 1408.0f, 1159.0f, 114.0f };
		doorInfo.doorHash = -562748873;
		break;

	case 93:
		doorInfo.modelInfoHash = $("v_ilev_ra_door1_r");
		doorInfo.coords = { 1408.0f, 1161.0f, 114.0f };
		doorInfo.doorHash = 1976429759;
		break;

	case 94:
		doorInfo.modelInfoHash = $("prop_gar_door_01");
		doorInfo.coords = { -1067.0f, -1666.0f, 5.0f };
		doorInfo.doorHash = 1341041543;
		break;

	case 95:
		doorInfo.modelInfoHash = $("prop_gar_door_02");
		doorInfo.coords = { -1065.0f, -1669.0f, 5.0f };
		doorInfo.doorHash = -1631467220;
		break;

	case 96:
		doorInfo.modelInfoHash = $("prop_map_door_01");
		doorInfo.coords = { -1104.66f, -1638.48f, 4.68f };
		doorInfo.doorHash = -1788473129;
		break;

	case 97:
		doorInfo.modelInfoHash = $("v_ilev_fib_door1");
		doorInfo.coords = { -31.72f, -1101.85f, 26.57f };
		doorInfo.doorHash = -1831288286;
		break;

	case 98:
		doorInfo.modelInfoHash = $("v_ilev_tort_door");
		doorInfo.coords = { 134.4f, -2204.1f, 7.52f };
		doorInfo.doorHash = 963876966;
		break;

	case 99:
		doorInfo.modelInfoHash = $("v_ilev_bl_shutter2");
		doorInfo.coords = { 3628.0f, 3747.0f, 28.0f };
		doorInfo.doorHash = 1773088812;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 100:
		doorInfo.modelInfoHash = $("v_ilev_bl_shutter2");
		doorInfo.coords = { 3621.0f, 3752.0f, 28.0f };
		doorInfo.doorHash = -1332101528;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 101:
		doorInfo.modelInfoHash = $("v_ilev_rc_door3_l");
		doorInfo.coords = { -608.73f, -1610.32f, 27.16f };
		doorInfo.doorHash = -1811763714;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 102:
		doorInfo.modelInfoHash = $("v_ilev_rc_door3_r");
		doorInfo.coords = { -611.32f, -1610.09f, 27.16f };
		doorInfo.doorHash = 1608500665;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 103:
		doorInfo.modelInfoHash = $("v_ilev_rc_door3_l");
		doorInfo.coords = { -592.94f, -1631.58f, 27.16f };
		doorInfo.doorHash = -1456048340;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 104:
		doorInfo.modelInfoHash = $("v_ilev_rc_door3_r");
		doorInfo.coords = { -592.71f, -1628.99f, 27.16f };
		doorInfo.doorHash = 943854909;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 105:
		doorInfo.modelInfoHash = $("v_ilev_ss_door04");
		doorInfo.coords = { 1991.0f, 3053.0f, 47.0f };
		doorInfo.doorHash = -89065356;
		break;

	case 106:
		doorInfo.modelInfoHash = 479144380;
		doorInfo.coords = { 1988.353f, 3054.411f, 47.3204f };
		doorInfo.doorHash = -925491840;
		break;

	case 107:
		doorInfo.modelInfoHash = $("prop_epsilon_door_l");
		doorInfo.coords = { -700.17f, 47.31f, 44.3f };
		doorInfo.doorHash = 1999872275;
		break;

	case 108:
		doorInfo.modelInfoHash = $("prop_epsilon_door_r");
		doorInfo.coords = { -697.94f, 48.35f, 44.3f };
		doorInfo.doorHash = 1999872275;
		break;

	case 109:
		doorInfo.modelInfoHash = $("v_ilev_cm_door1");
		doorInfo.coords = { 241.3574f, 361.0488f, 105.8963f };
		doorInfo.doorHash = 1538555582;
		break;

	case 110:
		doorInfo.modelInfoHash = $("prop_ch2_09c_garage_door");
		doorInfo.coords = { -689.11f, 506.97f, 110.64f };
		doorInfo.doorHash = -961994186;
		break;

	case 111:
		doorInfo.modelInfoHash = $("v_ilev_door_orangesolid");
		doorInfo.coords = { -1055.96f, -236.43f, 44.17f };
		doorInfo.doorHash = -1772472848;
		break;

	case 112:
		doorInfo.modelInfoHash = $("prop_magenta_door");
		doorInfo.coords = { 29.0f, 3661.0f, 41.0f };
		doorInfo.doorHash = -46374650;
		break;

	case 113:
		doorInfo.modelInfoHash = $("prop_cs4_05_tdoor");
		doorInfo.coords = { 32.0f, 3667.0f, 41.0f };
		doorInfo.doorHash = -358302761;
		break;

	case 114:
		doorInfo.modelInfoHash = $("v_ilev_housedoor1");
		doorInfo.coords = { 87.0f, -1959.0f, 21.0f };
		doorInfo.doorHash = -1237936041;
		break;

	case 115:
		doorInfo.modelInfoHash = $("v_ilev_fh_frntdoor");
		doorInfo.coords = { 0.0f, -1823.0f, 30.0f };
		doorInfo.doorHash = 1487374207;
		break;

	case 116:
		doorInfo.modelInfoHash = $("p_cut_door_03");
		doorInfo.coords = { 23.34f, -1897.6f, 23.05f };
		doorInfo.doorHash = -199126299;
		break;

	case 117:
		doorInfo.modelInfoHash = $("p_cut_door_02");
		doorInfo.coords = { 524.2f, 3081.14f, 41.16f };
		doorInfo.doorHash = -897071863;
		break;

	case 118:
		doorInfo.modelInfoHash = $("v_ilev_po_door");
		doorInfo.coords = { -1910.58f, -576.01f, 19.25f };
		doorInfo.doorHash = -864465775;
		break;

	case 119:
		doorInfo.modelInfoHash = $("prop_ss1_10_door_l");
		doorInfo.coords = { -720.39f, 256.86f, 80.29f };
		doorInfo.doorHash = -208439480;
		break;

	case 120:
		doorInfo.modelInfoHash = $("prop_ss1_10_door_r");
		doorInfo.coords = { -718.42f, 257.79f, 80.29f };
		doorInfo.doorHash = -1001088805;
		break;

	case 121:
		doorInfo.modelInfoHash = $("v_ilev_fibl_door02");
		doorInfo.coords = { 106.38f, -742.7f, 46.18f };
		doorInfo.doorHash = 756894459;
		break;

	case 122:
		doorInfo.modelInfoHash = $("v_ilev_fibl_door01");
		doorInfo.coords = { 105.76f, -746.65f, 46.18f };
		doorInfo.doorHash = 476981677;
		break;

	case 123:
		doorInfo.modelInfoHash = $("v_ilev_ct_door01");
		doorInfo.coords = { -2343.53f, 3265.37f, 32.96f };
		doorInfo.doorHash = 2081647379;
		break;

	case 124:
		doorInfo.modelInfoHash = $("v_ilev_ct_door01");
		doorInfo.coords = { -2342.23f, 3267.62f, 32.96f };
		doorInfo.doorHash = 2081647379;
		break;

	case 125:
		doorInfo.modelInfoHash = $("ap1_02_door_l");
		doorInfo.coords = { -1041.933f, -2748.167f, 22.0308f };
		doorInfo.doorHash = 169965357;
		break;

	case 126:
		doorInfo.modelInfoHash = $("ap1_02_door_r");
		doorInfo.coords = { -1044.841f, -2746.489f, 22.0308f };
		doorInfo.doorHash = 311232516;
		break;

	case 128:
		doorInfo.modelInfoHash = $("v_ilev_fb_doorshortl");
		doorInfo.coords = { -1045.12f, -232.004f, 39.4379f };
		doorInfo.doorHash = -1563127729;
		break;

	case 129:
		doorInfo.modelInfoHash = $("v_ilev_fb_doorshortr");
		doorInfo.coords = { -1046.516f, -229.3581f, 39.4379f };
		doorInfo.doorHash = 759145763;
		break;

	case 130:
		doorInfo.modelInfoHash = $("v_ilev_fb_door01");
		doorInfo.coords = { -1083.62f, -260.4167f, 38.1867f };
		doorInfo.doorHash = -84399179;
		break;

	case 131:
		doorInfo.modelInfoHash = $("v_ilev_fb_door02");
		doorInfo.coords = { -1080.974f, -259.0204f, 38.1867f };
		doorInfo.doorHash = -461898059;
		break;

	case 127:
		doorInfo.modelInfoHash = $("v_ilev_gtdoor");
		doorInfo.coords = { -1042.57f, -240.6f, 38.11f };
		doorInfo.doorHash = 1259065971;
		break;

	case 132:
		doorInfo.modelInfoHash = $("prop_damdoor_01");
		doorInfo.coords = { 1385.258f, -2079.949f, 52.7638f };
		doorInfo.doorHash = -884051216;
		break;

	case 133:
		doorInfo.modelInfoHash = $("v_ilev_genbankdoor2");
		doorInfo.coords = { 1656.57f, 4849.66f, 42.35f };
		doorInfo.doorHash = 243782214;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 134:
		doorInfo.modelInfoHash = $("v_ilev_genbankdoor1");
		doorInfo.coords = { 1656.25f, 4852.24f, 42.35f };
		doorInfo.doorHash = 714115627;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 135:
		doorInfo.modelInfoHash = -1184516519;
		doorInfo.coords = { -1051.402f, -474.6847f, 36.6199f };
		doorInfo.doorHash = 1668106976;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 1);
		break;

	case 136:
		doorInfo.modelInfoHash = -1184516519;
		doorInfo.coords = { -1049.285f, -476.6376f, 36.7584f };
		doorInfo.doorHash = 1382347031;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 1);
		break;

	case 137:
		doorInfo.modelInfoHash = 1230099731;
		doorInfo.coords = { -1210.957f, -580.8765f, 27.2373f };
		doorInfo.doorHash = -966790948;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 1);
		break;

	case 138:
		doorInfo.modelInfoHash = 1230099731;
		doorInfo.coords = { -1212.445f, -578.4401f, 27.2373f };
		doorInfo.doorHash = -2068750132;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 1);
		break;

	case 139:
		doorInfo.modelInfoHash = $("v_ilev_roc_door4");
		doorInfo.coords = { -565.1712f, 276.6259f, 83.2863f };
		doorInfo.doorHash = -1716533184;
		break;

	case 140:
		doorInfo.modelInfoHash = $("v_ilev_roc_door4");
		doorInfo.coords = { -561.2863f, 293.5043f, 87.7771f };
		doorInfo.doorHash = 2146505927;
		break;

	case 141:
		doorInfo.modelInfoHash = $("p_jewel_door_l");
		doorInfo.coords = { -631.96f, -236.33f, 38.21f };
		doorInfo.doorHash = 1874948872;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 142:
		doorInfo.modelInfoHash = $("p_jewel_door_r1");
		doorInfo.coords = { -630.43f, -238.44f, 38.21f };
		doorInfo.doorHash = -1965020851;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 145:
		doorInfo.modelInfoHash = -1743257725;
		doorInfo.coords = { 231.62f, 216.23f, 106.4f };
		doorInfo.doorHash = 1951546856;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 146:
		doorInfo.modelInfoHash = -1743257725;
		doorInfo.coords = { 232.72f, 213.88f, 106.4f };
		doorInfo.doorHash = -431382051;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 143:
		doorInfo.modelInfoHash = $("v_ilev_j2_door");
		doorInfo.coords = { 258.32f, 203.84f, 106.43f };
		doorInfo.doorHash = -293975210;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 144:
		doorInfo.modelInfoHash = $("v_ilev_j2_door");
		doorInfo.coords = { 260.76f, 202.95f, 106.43f };
		doorInfo.doorHash = -785215289;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 148:
		doorInfo.modelInfoHash = -222270721;
		doorInfo.coords = { 256.31f, 220.66f, 106.43f };
		doorInfo.doorHash = -366143778;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 147:
		doorInfo.modelInfoHash = $("v_ilev_bk_door");
		doorInfo.coords = { 266.36f, 217.57f, 110.43f };
		doorInfo.doorHash = 440819155;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 149:
		doorInfo.modelInfoHash = $("v_ilev_shrf2door");
		doorInfo.coords = { -442.66f, 6015.222f, 31.8663f };
		doorInfo.doorHash = -588495243;
		break;

	case 150:
		doorInfo.modelInfoHash = $("v_ilev_shrf2door");
		doorInfo.coords = { -444.4985f, 6017.06f, 31.8663f };
		doorInfo.doorHash = 1815504139;
		break;

	case 151:
		doorInfo.modelInfoHash = $("v_ilev_shrfdoor");
		doorInfo.coords = { 1855.685f, 3683.93f, 34.5928f };
		doorInfo.doorHash = 1344911780;
		break;

	case 152:
		doorInfo.modelInfoHash = $("prop_bhhotel_door_l");
		doorInfo.coords = { -1223.35f, -172.41f, 39.98f };
		doorInfo.doorHash = -320891223;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 153:
		doorInfo.modelInfoHash = $("prop_bhhotel_door_r");
		doorInfo.coords = { -1220.93f, -173.68f, 39.98f };
		doorInfo.doorHash = 1511747875;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 154:
		doorInfo.modelInfoHash = $("prop_bhhotel_door_l");
		doorInfo.coords = { -1211.99f, -190.57f, 39.98f };
		doorInfo.doorHash = -1517722103;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 155:
		doorInfo.modelInfoHash = $("prop_bhhotel_door_r");
		doorInfo.coords = { -1213.26f, -192.98f, 39.98f };
		doorInfo.doorHash = -1093199712;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 156:
		doorInfo.modelInfoHash = $("prop_bhhotel_door_l");
		doorInfo.coords = { -1217.77f, -201.54f, 39.98f };
		doorInfo.doorHash = 1902048492;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 157:
		doorInfo.modelInfoHash = $("prop_bhhotel_door_r");
		doorInfo.coords = { -1219.04f, -203.95f, 39.98f };
		doorInfo.doorHash = -444768985;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 158:
		doorInfo.modelInfoHash = $("prop_ch3_04_door_01l");
		doorInfo.coords = { 2514.32f, -317.34f, 93.32f };
		doorInfo.doorHash = 404057594;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 159:
		doorInfo.modelInfoHash = $("prop_ch3_04_door_01r");
		doorInfo.coords = { 2512.42f, -319.26f, 93.32f };
		doorInfo.doorHash = -1417472813;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 160:
		doorInfo.modelInfoHash = $("prop_ch3_01_trlrdoor_l");
		doorInfo.coords = { 2333.23f, 2574.97f, 47.03f };
		doorInfo.doorHash = -1376084479;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 161:
		doorInfo.modelInfoHash = $("prop_ch3_01_trlrdoor_r");
		doorInfo.coords = { 2329.65f, 2576.64f, 47.03f };
		doorInfo.doorHash = 457472151;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 162:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { 16.1279f, -1114.605f, 29.9469f };
		doorInfo.doorHash = 1071759151;
		break;

	case 163:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { 18.572f, -1115.495f, 29.9469f };
		doorInfo.doorHash = -2119023917;
		break;

	case 165:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { 1698.176f, 3751.506f, 34.8553f };
		doorInfo.doorHash = -1488490473;
		break;

	case 166:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { 1699.937f, 3753.42f, 34.8553f };
		doorInfo.doorHash = -511187813;
		break;

	case 167:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { 244.7274f, -44.0791f, 70.91f };
		doorInfo.doorHash = -248569395;
		break;

	case 168:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { 243.8379f, -46.5232f, 70.91f };
		doorInfo.doorHash = 989443413;
		break;

	case 169:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { 845.3624f, -1024.539f, 28.3448f };
		doorInfo.doorHash = 2022251829;
		break;

	case 170:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { 842.7684f, -1024.539f, 23.3448f };
		doorInfo.doorHash = 649820567;
		break;

	case 171:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { -326.1122f, 6075.27f, 31.6047f };
		doorInfo.doorHash = 537455378;
		break;

	case 172:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { -324.273f, 6077.109f, 31.6047f };
		doorInfo.doorHash = 1121431731;
		break;

	case 173:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { -665.2424f, -944.3256f, 21.9792f };
		doorInfo.doorHash = -1437380438;
		break;

	case 174:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { -662.6414f, -944.3256f, 21.9792f };
		doorInfo.doorHash = -946336965;
		break;

	case 175:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { -1313.826f, -389.1259f, 36.8457f };
		doorInfo.doorHash = 1893144650;
		break;

	case 176:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { -1314.465f, -391.6472f, 36.8457f };
		doorInfo.doorHash = 435841678;
		break;

	case 177:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { -1114.009f, 2689.77f, 18.7041f };
		doorInfo.doorHash = 948508314;
		break;

	case 178:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { -1112.071f, 2691.505f, 18.7041f };
		doorInfo.doorHash = -1796714665;
		break;

	case 179:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { -3164.845f, 1081.392f, 20.9887f };
		doorInfo.doorHash = -1155247245;
		break;

	case 180:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { -3163.812f, 1083.778f, 20.9887f };
		doorInfo.doorHash = 782482084;
		break;

	case 181:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { 2570.905f, 303.3556f, 108.8848f };
		doorInfo.doorHash = -1194470801;
		break;

	case 182:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { 2568.304f, 303.3556f, 108.8848f };
		doorInfo.doorHash = -2129698061;
		break;

	case 183:
		doorInfo.modelInfoHash = $("v_ilev_gc_door04");
		doorInfo.coords = { 813.1779f, -2148.27f, 29.7689f };
		doorInfo.doorHash = 1071759151;
		break;

	case 184:
		doorInfo.modelInfoHash = $("v_ilev_gc_door03");
		doorInfo.coords = { 810.5769f, -2148.27f, 29.7689f };
		doorInfo.doorHash = -2119023917;
		break;

	case 164:
		doorInfo.modelInfoHash = $("v_ilev_gc_door01");
		doorInfo.coords = { 6.8179f, -1098.209f, 29.9469f };
		doorInfo.doorHash = 1487704245;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 3);
		break;

	case 185:
		doorInfo.modelInfoHash = $("v_ilev_gc_door01");
		doorInfo.coords = { 827.5342f, -2160.493f, 29.7688f };
		doorInfo.doorHash = 1529812051;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 3);
		break;

	case 186:
		doorInfo.modelInfoHash = $("prop_lrggate_01c_l");
		doorInfo.coords = { -1107.01f, 289.38f, 64.76f };
		doorInfo.doorHash = 904342475;
		break;

	case 187:
		doorInfo.modelInfoHash = $("prop_lrggate_01c_r");
		doorInfo.coords = { -1101.62f, 290.36f, 64.76f };
		doorInfo.doorHash = -795418380;
		break;

	case 188:
		doorInfo.modelInfoHash = $("prop_lrggate_01c_l");
		doorInfo.coords = { -1138.64f, 300.82f, 67.18f };
		doorInfo.doorHash = -1502457334;
		break;

	case 189:
		doorInfo.modelInfoHash = $("prop_lrggate_01c_r");
		doorInfo.coords = { -1137.05f, 295.59f, 67.18f };
		doorInfo.doorHash = -1994188940;
		break;

	case 190:
		doorInfo.modelInfoHash = $("v_ilev_bl_doorel_l");
		doorInfo.coords = { -2053.16f, 3239.49f, 30.5f };
		doorInfo.doorHash = -621770121;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 191:
		doorInfo.modelInfoHash = $("v_ilev_bl_doorel_r");
		doorInfo.coords = { -2054.39f, 3237.23f, 30.5f };
		doorInfo.doorHash = 1018580481;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 192:
		doorInfo.modelInfoHash = $("v_ilev_cbankcountdoor01");
		doorInfo.coords = { -108.91f, 6469.11f, 31.91f };
		doorInfo.doorHash = 421926217;
		break;

	case 193:
		doorInfo.modelInfoHash = $("prop_fnclink_03gate5");
		doorInfo.coords = { -182.91f, 6168.37f, 32.14f };
		doorInfo.doorHash = -1331552374;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;
	case 196:
		doorInfo.modelInfoHash = $("v_ilev_csr_door_l");
		doorInfo.coords = { -59.89f, -1092.95f, 26.88f };
		doorInfo.doorHash = -293141277;
		break;

	case 197:
		doorInfo.modelInfoHash = $("v_ilev_csr_door_r");
		doorInfo.coords = { -60.55f, -1094.75f, 26.89f };
		doorInfo.doorHash = 506750037;
		break;

	case 194:
		doorInfo.modelInfoHash = $("v_ilev_csr_door_l");
		doorInfo.coords = { -39.13f, -1108.22f, 26.72f };
		doorInfo.doorHash = 1496005418;
		break;

	case 195:
		doorInfo.modelInfoHash = $("v_ilev_csr_door_r");
		doorInfo.coords = { -37.33f, -1108.87f, 26.72f };
		doorInfo.doorHash = -1863079210;
		break;

	case 198:
		doorInfo.modelInfoHash = $("prop_ron_door_01");
		doorInfo.coords = { 1943.73f, 3803.63f, 32.31f };
		doorInfo.doorHash = -2018911784;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 199:
		doorInfo.modelInfoHash = $("v_ilev_genbankdoor2");
		doorInfo.coords = { 316.39f, -276.49f, 54.52f };
		doorInfo.doorHash = -93934272;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 200:
		doorInfo.modelInfoHash = $("v_ilev_genbankdoor1");
		doorInfo.coords = { 313.96f, -275.6f, 54.52f };
		doorInfo.doorHash = 667682830;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 201:
		doorInfo.modelInfoHash = $("v_ilev_genbankdoor2");
		doorInfo.coords = { -2965.71f, 484.22f, 16.05f };
		doorInfo.doorHash = 1876735830;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 202:
		doorInfo.modelInfoHash = $("v_ilev_genbankdoor1");
		doorInfo.coords = { -2965.82f, 481.63f, 16.05f };
		doorInfo.doorHash = -2112857171;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 205:
		doorInfo.modelInfoHash = $("v_ilev_abbmaindoor");
		doorInfo.coords = { 962.1f, -2183.83f, 31.06f };
		doorInfo.doorHash = 2046930518;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 206:
		doorInfo.modelInfoHash = $("v_ilev_abbmaindoor2");
		doorInfo.coords = { 961.79f, -2187.08f, 31.06f };
		doorInfo.doorHash = 1208502884;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 207:
		doorInfo.modelInfoHash = $("prop_ch3_04_door_02");
		doorInfo.coords = { 2508.43f, -336.63f, 115.76f };
		doorInfo.doorHash = 1986432421;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 208:
		doorInfo.modelInfoHash = $("prop_ch1_07_door_01l");
		doorInfo.coords = { -2255.19f, 322.26f, 184.93f };
		doorInfo.doorHash = -722798986;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 209:
		doorInfo.modelInfoHash = $("prop_ch1_07_door_01r");
		doorInfo.coords = { -2254.06f, 319.7f, 184.93f };
		doorInfo.doorHash = 204301578;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 210:
		doorInfo.modelInfoHash = $("prop_ch1_07_door_01l");
		doorInfo.coords = { -2301.13f, 336.91f, 184.93f };
		doorInfo.doorHash = -320140460;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 211:
		doorInfo.modelInfoHash = $("prop_ch1_07_door_01r");
		doorInfo.coords = { -2298.57f, 338.05f, 184.93f };
		doorInfo.doorHash = 65222916;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 212:
		doorInfo.modelInfoHash = $("prop_ch1_07_door_01l");
		doorInfo.coords = { -2222.32f, 305.86f, 184.93f };
		doorInfo.doorHash = -920027322;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 213:
		doorInfo.modelInfoHash = $("prop_ch1_07_door_01r");
		doorInfo.coords = { -2221.19f, 303.3f, 184.93f };
		doorInfo.doorHash = -58432001;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 214:
		doorInfo.modelInfoHash = $("prop_ch1_07_door_01l");
		doorInfo.coords = { -2280.6f, 265.43f, 184.93f };
		doorInfo.doorHash = -2007378629;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 215:
		doorInfo.modelInfoHash = $("prop_ch1_07_door_01r");
		doorInfo.coords = { -2278.04f, 266.57f, 184.93f };
		doorInfo.doorHash = 418772613;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 216:
		doorInfo.modelInfoHash = $("prop_gar_door_04");
		doorInfo.coords = { 778.31f, -1867.49f, 30.66f };
		doorInfo.doorHash = 1679064921;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 217:
		doorInfo.modelInfoHash = $("prop_gate_tep_01_l");
		doorInfo.coords = { -721.35f, 91.01f, 56.68f };
		doorInfo.doorHash = 412198396;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 218:
		doorInfo.modelInfoHash = $("prop_gate_tep_01_r");
		doorInfo.coords = { -728.84f, 88.64f, 56.68f };
		doorInfo.doorHash = -1053755588;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 219:
		doorInfo.modelInfoHash = $("prop_artgallery_02_dr");
		doorInfo.coords = { -2287.62f, 363.9f, 174.93f };
		doorInfo.doorHash = -53446139;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 220:
		doorInfo.modelInfoHash = $("prop_artgallery_02_dl");
		doorInfo.coords = { -2289.78f, 362.91f, 174.93f };
		doorInfo.doorHash = 1333960556;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 221:
		doorInfo.modelInfoHash = $("prop_artgallery_02_dr");
		doorInfo.coords = { -2289.86f, 362.88f, 174.93f };
		doorInfo.doorHash = -41786493;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 222:
		doorInfo.modelInfoHash = $("prop_artgallery_02_dl");
		doorInfo.coords = { -2292.01f, 361.89f, 174.93f };
		doorInfo.doorHash = 1750120734;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 223:
		doorInfo.modelInfoHash = $("prop_fnclink_07gate1");
		doorInfo.coords = { 1803.94f, 3929.01f, 33.72f };
		doorInfo.doorHash = 1661506222;
		break;

	case 203:
		doorInfo.modelInfoHash = $("v_ilev_genbankdoor2");
		doorInfo.coords = { -348.81f, -47.26f, 49.39f };
		doorInfo.doorHash = -2116116146;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 204:
		doorInfo.modelInfoHash = $("v_ilev_genbankdoor1");
		doorInfo.coords = { -351.26f, -46.41f, 49.39f };
		doorInfo.doorHash = -74083138;
		GAMEPLAY::SET_BIT(&doorInfo.doorState, 2);
		break;

	case 224:
		doorInfo.modelInfoHash = $("prop_abat_slide");
		doorInfo.coords = { 962.9084f, -2105.814f, 34.6432f };
		doorInfo.doorHash = -1670085357;
		break;
	}
	return doorInfo;
}

#include <array>
struct door_t
{
	char* doorName;
	CVector3 coords;
};

const std::array<door_t, 487> doors
{ {
		// Reference from am_doors
		{ "prop_com_gar_door_01", { 1204.5699462890625f, 3110.4f, 6.57f }},
		{ "v_ilev_rc_door2",{ 467.3716125488281f, -1014.4520263671875f, 26.53619956970215f } },
		{ "v_ilev_rc_door2",{ 469.9678955078125f, -1014.4520263671875f, 26.53619956970215f } },
		{ "v_ilev_gtdoor02",{ 464.3612976074219f, -984.6779174804688f, 43.834400177f } },
		{ "v_ilev_ph_door01",{ 434.75f, -980.6099853515625f, 30.84f } },
		{ "v_ilev_ph_door002",{ 434.75f, -983.219970703125f, 30.84f } },
		{ "prop_biolab_g_door",{ 3589.1f, 3671.5f, 35.0f } },
		{ "prop_biolab_g_door",{ 3587.6f, 3663.3f, 35.0f } },
		{ "v_ilev_bl_shutter2",{ 3627.7099609375f, 3746.719970703125f, 27.69f } },
		{ "v_ilev_bl_shutter2",{ 3620.84f, 3751.53f, 27.69f } },
		{ "v_ilev_abbmaindoor",{ 962.1f, -2183.83f, 31.06f } },
		{ "v_ilev_abbmaindoor2",{ 961.7899780273438f, -2187.08f, 31.06f } },
		{ "prop_gate_airport_01",{ -1138.469970703125f, -2730.449951171875f, 12.95f } },
		{ "prop_gate_airport_01",{ -1151.204345703125f, -2723.102294921875f, 12.946f } },
		{ "prop_gate_airport_01",{ -977.5173950195312f, -2837.264404296875f, 12.954899787902832f } },
		{ "prop_gate_airport_01",{ -990.2963256835938f, -2829.88720703125f, 12.949899673461914f } },
		{ "prop_facgate_01",{ -1213.4f, -2079.3f, 12.907f } },
		{ "prop_fnclink_02gate6_r",{ -967.4473266601562f, -2778.4951171875f, 14.409f } },
		{ "prop_fnclink_02gate6_l",{ -974.5734252929688f, -2774.380859375f, 14.409899711608887f } },
		{ "prop_fnclink_02gate6_r",{ -971.101806640625f, -2776.385498046875f, 14.409f } },
		{ "prop_fnclink_02gate6_l",{ -970.9188232421875f, -2776.49072265625f, 14.409f } },
		{ "prop_fnclink_02gate6_r",{ -935.21142578125f, -2767.396728515625f, 14.388199806213379f } },
		{ "prop_fnclink_02gate6_l",{ -933.1580810546875f, -2763.95458984375f, 14.388199806213379f } },
		{ "prop_fnclink_02gate6_r",{ -933.0535278320312f, -2763.779296875f, 14.388199806213379f } },
		{ "prop_fnclink_02gate6_l",{ -931.0001831054688f, -2760.337158203125f, 14.388199806213379f } },
		{ "prop_fnclink_02gate6_r",{ -773.2437744140625f, -2842.677490234375f, 14.271499633789062f } },
		{ "prop_fnclink_02gate6_l",{ -769.7720947265625f, -2844.681884765625f, 14.271499633789062f } },
		{ "prop_fnclink_02gate6_r",{ -769.6071166992188f, -2844.777099609375f, 14.271499633789062f } },
		{ "prop_fnclink_02gate6_l",{ -766.1353759765625f, -2846.781494140625f, 14.271499633789062f } },
		{ "prop_fnclink_09gate1",{ -828.9456176757812f, -2964.3037109375f, 14.275799751281738f } },
		{ "prop_fnclink_09gate1",{ 907.8f, -3100.8740234375f, 14.280799865722656f } },
		{ "prop_fnclink_09gate1",{ -833.4395141601562f, -3186.70947265625f, 14.267f } },
		{ "prop_fnclink_09gate1",{ -830.0543823242188f, -3391.162841796875f, 14.197199821472168f } },
		{ "prop_fnclink_09gate1",{ -913.0833740234375f, -3534.9697265625f, 14.1924f } },
		{ "prop_fnclink_09gate1",{ -971.6149291992188f, -3549.152099609375f, 14.2727f } },
		{ "prop_fnclink_09gate1",{ -1017.6920776367188f, -3563.21728515625f, 14.2767f } },
		{ "prop_fnclink_09gate1",{ -1146.8314208984375f, -3546.63818359375f, 14.259499549865723f } },
		{ "prop_fnclink_09gate1",{ -1260.8966064453125f, -3480.763916015625f, 14.1721f } },
		{ "prop_fnclink_09gate1",{ -1351.1328125f, -3404.16162109375f, 14.1721f } },
		{ "prop_fnclink_09gate1",{ -1409.50732421875f, -3370.46142578125f, 14.2068f } },
		{ "prop_fnclink_09gate1",{ -1831.803955078125f, -3224.96630859375f, 14.3119f } },
		{ "prop_fnclink_09gate1",{ -1942.289306640625f, -3161.1904296875f, 14.2981f } },
		{ "prop_fnclink_09gate1",{ -1927.5634765625f, -3076.26904296875f, 14.456899642944336f } },
		{ "prop_fnclink_09gate1",{ -1951.695556640625f, -3003.845703125f, 14.4418f } },
		{ "prop_fnclink_09gate1",{ -1836.875f, -2804.968994140625f, 14.4557f } },
		{ "prop_fnclink_09gate1",{ -1802.6915283203125f, -2745.761474609375f, 14.448f } },
		{ "prop_facgate_01",{ -1099.53076171875f, -2020.8026123046875f, 12.1745f } },
		{ "prop_facgate_01",{ -994.4995727539062f, -2341.648193359375f, 12.9448f } },
		{ "prop_facgate_01",{ -984.0709228515625f, -2348.4f, 12.9448f } },
		{ "prop_gate_airport_01",{ -1008.0708f, -2406.7509765625f, 12.977f } },
		{ "prop_gate_airport_01",{ -1015.4854125976562f, -2419.582763671875f, 12.9586f } },
		{ "v_ilev_lester_doorfront",{ 1273.8199462890625f, 1720.6999999999998f, 54.919999999999995f } },
		{ "v_ilev_tort_door",{ 134.39999999999998f, -2204.1f, 7.52f } },
		{ "v_ilev_janitor_frontdoor",{ -107.5373f, -9.018099784851074f, 70.6707992553711f } },
		{ "v_ilev_deviantfrontdoor",{ -128.33f, -1457.17f, 37.94f } },
		{ "v_ilev_po_door",{ -1910.5799560546875f, -576.01f, 19.25f } },
		{ "v_ilev_ra_door3",{ 1395.92f, 1142.9f, 114.79f } },
		{ "v_ilev_ra_door3",{ 1395.92f, 1140.6999999999998f, 114.79f } },
		{ "v_ilev_ra_door1_l",{ 1390.52f, 1163.43994140625f, 114.38f } },
		{ "v_ilev_ra_door1_r",{ 1390.52f, 1161.24f, 114.38f } },
		{ "v_ilev_ra_door1_l",{ 1408.0699462890625f, 1158.969970703125f, 114.48f } },
		{ "v_ilev_ra_door1_r",{ 1408.0699462890625f, 1161.17f, 114.48f } },
		{ "v_ilev_ra_door1_l",{ 1409.29f, 1146.25f, 114.49f } },
		{ "v_ilev_ra_door1_r",{ 1409.29f, 1148.449951171875f, 114.49f } },
		{ "v_ilev_bl_door_l",{ -1387.05f, -586.58f, 30.47f } },
		{ "v_ilev_bl_door_r",{ 1389.24f, -588.0f, 30.47f } },
		{ "prop_sec_gate_01c",{ 25.03f, 664.6f, 31.04f } },
		{ "prop_sec_gate_01d",{ -72.75f, -682.1699829101562f, 33.27f } },
		{ "v_ilev_bank4door02",{ -111.48f, 6463.93994140625f, 31.985f } },
		{ "v_ilev_bank4door01",{ -109.65f, 6462.10986328125f, 31.985f } },
		{ "hei_v_ilev_bk_gate_pris",{ 256.3116149902344f, 220.65789794921875f, 106.42960357666016f } },
		{ "hei_v_ilev_bk_gate2_pris",{ 262.1980895996094f, 222.518798828125f, 106.42960357666016f } },
		{ "v_ilev_genbankdoor1",{ -2965.821044921875f, 481.63f, 16.048f } },
		{ "v_ilev_genbankdoor2",{ -2965.7099609375f, 484.218994140625f, 16.048f } },
		{ "v_ilev_genbankdoor1",{ 1176.49f, 2703.610107421875f, 38.44f } },
		{ "v_ilev_genbankdoor2",{ 1173.9f, 2703.610107421875f, 38.44f } },
		{ "v_ilev_genbankdoor1",{ 1656.25f, 4852.240234375f, 42.35f } },
		{ "v_ilev_genbankdoor2",{ 1656.5699462890625f, 4849.66015625f, 42.35f } },
		{ "v_ilev_genbankdoor1",{ -1215.39f, -328.5199890136719f, 38.13f } },
		{ "v_ilev_genbankdoor2",{ -1213.0699462890625f, -327.35f, 38.13f } },
		{ "v_ilev_genbankdoor1",{ 149.63f, -1037.22998046875f, 29.72f } },
		{ "v_ilev_genbankdoor2",{ 152.06f, 1038.12f, 29.72f } },
		{ "v_ilev_genbankdoor1",{ 313.96f, -275.6f, 54.52f } },
		{ "v_ilev_genbankdoor2",{ 316.39f, 276.49f, 54.52f } },
		{ "v_ilev_genbankdoor2",{ -348.8109130859375f, -47.2621f, 49.3876f } },
		{ "v_ilev_genbankdoor1",{ -351.2597961425781f, -46.4122f, 49.3876f } },
		{ "v_ilev_fibl_door02",{ 106.38f, -742.7f, 46.18f } },
		{ "v_ilev_fibl_door01",{ 105.76f, -746.65f, 46.18f } },
		{ "prop_ch3_01_trlrdoor_l",{ 2333.22998046875f, 2574.969970703125f, 47.03f } },
		{ "prop_ch3_01_trlrdoor_r",{ 2329.64990234375f, 2576.639892578125f, 47.03f } },
		{ "v_ilev_fh_frntdoor",{ 0.2169f, -1823.3031f, 29.63909912109375f } },
		{ "v_ilev_bl_doorel_l",{ -2053.159912109375f, 3239.49f, 30.5f } },
		{ "v_ilev_bl_doorel_r",{ -2054.389892578125f, 3237.22998046875f, 30.5f } },
		{ "prop_cs4_05_tdoor",{ 31.918f, 3666.853515625f, 40.8586f } },
		{ "prop_magenta_door",{ 29.102f, 3661.4892578125f, 40.854698181152344f } },
		{ "hei_prop_heist_cutscene_doorc_r",{ 610.593994140625f, -421.8299865722656f, 24.979f } },
		{ "hei_prop_heist_cutscene_doorc_l",{ 610.8740234375f, -419.364990234375f, 24.979f } },
		{ "prop_gate_docks_ld",{ 492.0f, -3116.0f, 5.0f } },
		{ "prop_gate_docks_ld",{ 476.0f, -3116.0f, 5.0f } },
		{ "prop_fnclink_03gate2",{ -687.72998046875f, -2458.82f, 12.899999999999999f } },
		{ "prop_fnclink_03gate2",{ -697.82f, -1226.5f, 12.91f } },
		{ "prop_fnclink_03gate2",{ -692.77f, -2455.929931640625f, 12.899999999999999f } },
		{ "prop_gate_airport_01",{ 891.93f, -2748.7099609375f, 12.95f } },
		{ "prop_gate_airport_01",{ -896.46f, -2746.419921875f, 12.95f } },
		{ "prop_gate_airport_01",{ -859.72998046875f, 2683.9f, 12.76f } },
		{ "prop_facgate_01",{ -1019.6099853515625f, -1897.02f, 13.37f } },
		{ "prop_facgate_01",{ -160.88f, 2636.2f, 5.03f } },
		{ "prop_facgate_01",{ -148.71f, 2636.2f, 5.03f } },
		{ "prop_facgate_01",{ 237.78f, -2936.9599609375f, 5.05f } },
		{ "prop_gate_docks_ld",{ 188.28f, -2204.610107421875f, 9.3f } },
		{ "prop_gate_docks_ld",{ 127.56f, -2185.29f, 9.3f } },
		{ "prop_gate_docks_ld",{ -33.22f, -2141.27f, 9.3f } },
		{ "prop_facgate_01",{ 1000.68f, 2454.99f, 27.57f } },
		{ "prop_facgate_01",{ 1012.9099731445312f, -2456.22998046875f, 27.53f } },
		{ "prop_facgate_01",{ 1028.0799560546875f, -2364.719970703125f, 29.52f } },
		{ "prop_facgate_01",{ 1033.219970703125f, -2299.1f, 29.52f } },
		{ "prop_facgate_01",{ 1022.4f, -2417.1298828125f, 28.13f } },
		{ "prop_gate_military_01",{ 2485.43994140625f, 432.71f, 91.97f } },
		{ "prop_gate_prison_01",{ 2485.09f, 335.84f, 91.98f } },
		{ "prop_facgate_01",{ 2491.969970703125f, -303.48f, 91.99f } },
		{ "prop_facgate_01",{ 1944.8599853515625f, -957.69f, 78.15f } },
		{ "prop_facgate_01",{ 1956.74f, -956.8599853515625f, 78.15f } },
		{ "prop_gate_prison_01",{ 1845.0f, 2604.81f, 44.64f } },
		{ "prop_gate_prison_01",{ 1818.55f, 2604.81f, 44.6f } },
		{ "prop_gate_prison_01",{ 1799.74f, 2616.97998046875f, 44.61f } },
		{ "prop_gate_prison_01",{ 1813.4f, 2488.6298828125f, 44.47f } },
		{ "prop_gate_prison_01",{ 1808.8199462890625f, 2474.8798828125f, 44.47f } },
		{ "prop_gate_prison_01",{ 1762.18994140625f, 2426.72998046875f, 44.44f } },
		{ "prop_gate_prison_01",{ 1749.219970703125f, 2420.28f, 44.43f } },
		{ "prop_gate_prison_01",{ 1667.6099853515625f, 2408.110107421875f, 44.419999999999995f } },
		{ "prop_gate_prison_01",{ 1661.76f, 2748.32f, 44.43f } },
		{ "prop_gate_prison_01",{ 1653.2099609375f, 2409.7099609375f, 44.419999999999995f } },
		{ "prop_gate_prison_01",{ 1558.14f, 2469.8701171875f, 44.4f } },
		{ "prop_gate_prison_01",{ 1551.18f, 2482.58f, 44.4f } },
		{ "prop_gate_prison_01",{ 1547.41f, 2576.6f, 44.51f } },
		{ "prop_gate_prison_01",{ 1547.99f, 2591.08f, 44.51f } },
		{ "prop_gate_prison_01",{ 1576.18994140625f, 2667.22998046875f, 44.51f } },
		{ "prop_gate_prison_01",{ 1584.63f, 2679.02f, 44.51f } },
		{ "prop_gate_prison_01",{ 1648.949951171875f, 2741.55f, 44.45f } },
		{ "prop_gate_prison_01",{ 1762.5799560546875f, 2751.9f, 44.45f } },
		{ "prop_gate_prison_01",{ 1776.26f, 2747.1f, 44.43f } },
		{ "prop_gate_prison_01",{ 1829.8f, 2702.919921875f, 44.45f } },
		{ "prop_gate_prison_01",{ 1834.92f, 2689.3701171875f, 44.45f } },
		{ "prop_fnclink_03gate1",{ 222.07f, 2013.99f, 18.41f } },
		{ "prop_fnclink_03gate1",{ 210.92999999999998f, -2022.5799560546875f, 17.65f } },
		{ "prop_facgate_01b",{ 459.7f, -2002.93994140625f, 22.07f } },
		{ "prop_facgate_01",{ 451.45f, -1994.01f, 22.07f } },
		{ "prop_facgate_01b",{ 455.74f, -1944.8499755859375f, 23.66f } },
		{ "prop_facgate_01",{ 466.76f, -1939.65f, 23.66f } },
		{ "prop_facgate_01b",{ 539.83f, -1901.88f, 24.22f } },
		{ "prop_facgate_01",{ 550.87f, -1896.75f, 24.15f } },
		{ "prop_facgate_01",{ 564.12f, -1903.0f, 23.71f } },
		{ "prop_facgate_01",{ 567.4f, -1908.219970703125f, 23.71f } },
		{ "prop_fnclink_03gate1",{ 1081.65f, -1818.42f, 36.43f } },
		{ "prop_facgate_01",{ 987.52f, -1176.8199462890625f, 24.55f } },
		{ "prop_facgate_01",{ 705.64f, -1319.67f, 24.96f } },
		{ "prop_facgate_01",{ 712.95f, -1329.42f, 24.99f } },
		{ "prop_facgate_01",{ 796.46f, 921.49f, 24.4f } },
		{ "prop_facgate_01",{ 795.78f, -909.33f, 24.36f } },
		{ "prop_facgate_01",{ 795.1599731445312f, -896.64f, 24.1f } },
		{ "prop_facgate_01",{ 794.47998046875f, -884.47998046875f, 24.06f } },
		{ "prop_fnclink_03gate2",{ 546.1699829101562f, -1869.93994140625f, 24.53f } },
		{ "prop_fnclink_03gate1",{ 492.3299865722656f, -1410.469970703125f, 28.43f } },
		{ "prop_fnclink_03gate1",{ 484.57f, -1408.5f, 28.48f } },
		{ "prop_fnclink_03gate1",{ 469.3299865722656f, 1272.6999999999998f, 28.82f } },
		{ "prop_fnclink_03gate1",{ 487.87f, -1272.5899658203125f, 28.86f } },
		{ "prop_facgate_08",{ 488.89f, -1011.6699829101562f, 27.14f } },
		{ "prop_fnclink_03gate1",{ 527.5499877929688f, -1597.3299560546875f, 28.39f } },
		{ "prop_fnclink_03gate1",{ 544.94f, -1649.75f, 27.49f } },
		{ "prop_fnclink_03gate2",{ 151.35f, -1668.64f, 28.77f } },
		{ "prop_fnclink_03gate1",{ 47.2f, -1448.3f, 28.44f } },
		{ "prop_lrggate_03a",{ 1876.37f, 194.85f, 83.33f } },
		{ "prop_lrggate_03a",{ -1868.31f, 183.79f, 83.34f } },
		{ "prop_lrggate_03a",{ -1452.43f, 37.03f, 51.75f } },
		{ "prop_lrggate_03a",{ 1483.1f, 46.93f, 53.26f } },
		{ "prop_lrggate_03a",{ -1507.469970703125f, 39.16f, 54.41f } },
		{ "prop_lrggate_03a",{ -1528.3599853515625f, 35.77f, 55.78f } },
		{ "prop_lrggate_02_ld",{ -1583.28f, 40.14f, 59.32f } },
		{ "prop_lrggate_02_ld",{ -1616.22998046875f, 79.78f, 60.78f } },
		{ "prop_lrggate_03a",{ -924.97998046875f, 9.03f, 43.28f } },
		{ "prop_lrggate_03a",{ -905.7f, 14.66f, 45.66f } },
		{ "prop_lrggate_02_ld",{ -844.0499877929688f, 155.96f, 66.03f } },
		{ "prop_lrggate_04a",{ -914.32f, 184.54f, 68.42f } },
		{ "v_ilev_trev_doorfront",{ -1149.7099609375f, -1521.0899658203125f, 10.79f } },
		{ "v_ilev_ct_door01",{ -2343.53f, 3265.3701171875f, 32.96f } },
		{ "v_ilev_ct_door01",{ -2342.22998046875f, 3267.6201171875f, 32.96f } },
		{ "v_ilev_housedoor1",{ 347.8677978515625f, -1003.31591796875f, -99.09519958496094f } },
		{ "v_ilev_mm_door",{ 257.28961181640625f, -1001.2545776367188f, -98.85870361328125f } },
		{ "v_ilev_mm_doorw",{ 348.2156982421875f, -993.1121826171875f, 99.043f } },
		{ "v_ilev_fh_bedrmdoor",{ -789.3016967773438f, 332.01190185546875f, 201.55960083f } },
		{ "prop_ld_garaged_01",{ -815.3281860351562f, 185.9571075439453f, 72.99f } },
		{ "v_ilev_mm_windowwc",{ -802.7332763671875f, 167.5041046142578f, 77.5823974609375f } },
		{ "v_ilev_mm_doorm_l",{ -816.716f, 179.098f, 72.84f } },
		{ "v_ilev_mm_doorm_r",{ -816.1068115234375f, 177.51089477539062f, 72.8274f } },
		{ "prop_bh1_48_backdoor_l",{ -796.565673828125f, 177.22140502929688f, 73.04049682617188f } },
		{ "prop_bh1_48_backdoor_r",{ -794.505126953125f, 178.0124053955078f, 73.04049682617188f } },
		{ "prop_bh1_48_backdoor_l",{ -793.394287109375f, 180.50750732421875f, 73.04049682617188f } },
		{ "prop_bh1_48_backdoor_r",{ -794.185302734375f, 182.5679931640625f, 73.04049682617188f } },
		{ "v_ilev_mm_door",{ -806.2816772460938f, 187.02459716796875f, 72.624f } },
		{ "prop_gate_cult_01_l",{ -1041.267578125f, 4906.0966796875f, 209.2f } },
		{ "prop_gate_cult_01_r",{ -1044.7490234375f, 4914.9716796875f, 209.19320678710938f } },
		{ "v_ilev_trevtraildr",{ 1972.7689208984375f, 3815.365966796875f, 33.663299560546875f } },
		{ "v_ilev_fa_frontdoor",{ -14.8689f, -1441.18212890625f, 31.192f } },
		{ "v_ilev_ss_door8",{ 716.7808227539062f, -975.4207153320312f, 25.057f } },
		{ "v_ilev_ss_door7",{ 719.3817749023438f, -975.4185180664062f, 25.005699157714844f } },
		{ "v_ilev_ss_door02",{ 710.0f, -964.0f, 31.0f } },
		{ "v_ilev_bl_doorel_l",{ 3539.0f, 3673.7f, 20.0f } },
		{ "v_ilev_bl_doorel_r",{ 3541.6f, 3673.2f, 20.0f } },
		{ "v_ilev_store_door",{ 708.0f, -962.0f, 31.0f } },
		{ "p_jewel_door_l",{ -631.9553833f, -236.3332977294922f, 38.20650100708f } },
		{ "p_jewel_door_r1",{ -630.426513671875f, -238.4375f, 38.20650100708f } },
		{ "v_ilev_ss_door04",{ 1395.6134033203125f, 3609.326904296875f, 35.13079833984375f } },
		{ "v_ilev_ss_doorext",{ 1388.4986572265625f, 3614.82763671875f, 39.09189987182617f } },
		{ "v_ilev_ss_doorext",{ 1399.6995849609375f, 3607.762939453125f, 39.09189987182617f } },
		{ "prop_com_gar_door_01",{ 484.564208984375f, -1315.573974609375f, 30.2033f } },
		{ "v_ilev_cs_door",{ 482.8111877441406f, -1311.953f, 29.35059928894043f } },
		{ "v_ilev_epsstoredoor",{ 241.3621063232422f, 361.0470886230469f, 105.00299835205078f } },
		{ "v_ilev_fh_frontdoor",{ 7.5179f, 539.526f, 176.1781f } },
		{ "prop_ch_025c_g_door_01",{ 18.6504f, 546.34f, 176.3448028564453f } },
		{ "v_ilev_csr_door_l",{ -59.893f, -1092.9517822265625f, 26.8836f } },
		{ "v_ilev_csr_door_r",{ -60.545799255371094f, -1094.7489013671875f, 26.8887f } },
		{ "v_ilev_csr_door_l",{ -38.464f, -1108.4461669921875f, 26.7198f } },
		{ "v_ilev_csr_door_r",{ -36.6614990234375f, -1109.1015625f, 26.7198f } },
		{ "v_ilev_rc_door3_l",{ -608.7288818359375f, -1610.3153076171875f, 27.158899307250977f } },
		{ "v_ilev_rc_door3_r",{ -611.32f, -1610.088623046875f, 27.158899307250977f } },
		{ "v_ilev_rc_door3_l",{ -592.9376220703125f, -1631.5770263671875f, 27.159299850463867f } },
		{ "v_ilev_rc_door3_r",{ -592.7108764648438f, -1628.9859619140625f, 27.159299850463867f } },
		{ "v_ilev_shrf2door",{ -442.66f, 6015.2216796875f, 31.8663f } },
		{ "v_ilev_shrf2door",{ -444.4985046386719f, 6017.06f, 31.8663f } },
		{ "v_ilev_shrfdoor",{ 1855.684814453125f, 3683.93017578125f, 34.5928f } },
		{ "prop_gar_door_01",{ -1067.00244140625f, -1665.609130859375f, 4.7898f } },
		{ "prop_gar_door_02",{ -1064.76318359375f, -1668.7623291015625f, 4.8084f } },
		{ "prop_gar_door_03_ld",{ -1074.648193359375f, -1676.13134765625f, 4.684f } },
		{ "prop_gar_door_05",{ 201.39999999999998f, -153.36520385742188f, 57.85219955444336f } },
		{ "v_ilev_fb_doorshortl",{ -1045.119873046875f, -232.00399780273438f, 39.4379f } },
		{ "v_ilev_fb_doorshortr",{ -1046.51611328125f, -229.35809326171875f, 39.4379f } },
		{ "v_ilev_fb_door01",{ -1083.6201171875f, -260.41668701171875f, 38.18669891357422f } },
		{ "v_ilev_fb_door02",{ -1080.974365234375f, -259.0203857421875f, 38.18669891357422f } },
		{ "v_ilev_gtdoor",{ -1042.5699462890625f, -240.6f, 38.11f } },
		{ "v_ilev_roc_door4",{ -565.1712036132812f, 276.625885f, 83.2863f } },
		{ "v_ilev_roc_door4",{ -561.2863159179688f, 293.5043029785156f, 87.777099609375f } },
		{ "prop_ron_door_01",{ 1065.2371826171875f, -2006.0791015625f, 32.2328987121582f } },
		{ "prop_ron_door_01",{ 1083.5472412109375f, -1975.4354248046875f, 31.622f } },
		{ "prop_ron_door_01",{ 1085.307f, -2018.5614013671875f, 41.62889862060547f } },
		{ "v_ilev_housedoor1",{ 86.66239929199219f, -1959.47705078125f, 21.2152f } },
		{ "prop_abat_slide",{ 962.9083862304688f, -2105.813720703125f, 34.6432f } },
		{ "prop_gar_door_04",{ 778.31f, 1867.49f, 30.66f } },
		{ "prop_ch3_04_door_01l",{ 2514.32f, 317.34f, 93.32f } },
		{ "prop_ch3_04_door_01r",{ 2512.419921875f, -319.26f, 93.32f } },
		{ "prop_sc1_12_door",{ -58.47f, -1530.51f, 34.54f } },
		{ "prop_ch1_07_door_01l",{ -2255.19384765625f, 322.2593078613281f, 184.92640686035156f } },
		{ "prop_ch1_07_door_01r",{ -2254.0556640625f, 319.7f, 184.92640686035156f } },
		{ "prop_ch1_07_door_01l",{ -2280.596923828125f, 265.432f, 184.92599487304688f } },
		{ "prop_ch1_07_door_01r",{ -2278.0390625f, 266.57f, 184.92599487304688f } },
		{ "prop_facgate_08",{ 413.3648986816406f, -1620.0333251953125f, 28.3416f } },
		{ "prop_facgate_08",{ 418.28961181640625f, -1651.396240234375f, 28.29509925842285f } },
		{ "prop_facgate_07b",{ 397.8846130371094f, -1607.3837890625f, 28.33f } },
		{ "prop_sc1_21_g_door_01",{ -25.28f, -1431.06f, 30.84f } },
		{ "prop_ss1_14_garage_door",{ -62.38f, 352.71728515625f, 113.2499f } },
		{ "prop_motel_door_09",{ 549.2567138671875f, -1773.114990234375f, 33.730899810791016f } },
		{ "prop_ss1_10_door_l",{ -720.39f, 256.8599853515625f, 80.29f } },
		{ "prop_ss1_10_door_r",{ -718.4199829101562f, 257.79f, 80.29f } },

		////////////////////////////////////////////////////////////////////

		// Reference from ambient_solomon

		{ "v_ilev_bs_door",{ 133.0f, -1711.0f, 29.0f } },
		{ "v_ilev_bs_door",{ -1287.8568115234375f, -1115.7415771484375f, 7.1401f } },
		{ "v_ilev_bs_door",{ 1932.9517822265625f, 3725.153564453125f, 32.9944f } },
		{ "v_ilev_bs_door",{ 1207.8731689453125f, -470.06298828125f, 66.358f } },
		{ "v_ilev_bs_door",{ -29.869199752807617f, -148.1571044921875f, 57.22650146484375f } },
		{ "v_ilev_bs_door",{ -280.78509521484375f, 6232.78173828125f, 31.8455f } },
		{ "v_ilev_hd_door_l",{ -824.0f, -187.0f, 38.0f } },
		{ "v_ilev_hd_door_r",{ -823.0f, -188.0f, 38.0f } },
		{ "v_ilev_cs_door01",{ 82.318603515625f, -1392.7518310546875f, 29.5261f } },
		{ "v_ilev_cs_door01_r",{ 82.318603515625f, -1390.47583f, 29.5261f } },
		{ "v_ilev_cs_door01",{ 1686.983154296875f, 4821.74072265625f, 42.2131f } },
		{ "v_ilev_cs_door01_r",{ 1687.28173828125f, 4819.484375f, 42.2131f } },
		{ "v_ilev_cs_door01",{ 418.6369934082031f, -806.4569702148438f, 29.6396f } },
		{ "v_ilev_cs_door01_r",{ 418.6369934082031f, -808.7329711914062f, 29.6396f } },
		{ "v_ilev_cs_door01",{ -1096.6612548828125f, 2705.4458f, 19.25779914855957f } },
		{ "v_ilev_cs_door01_r",{ -1094.9652099609375f, 2706.963623046875f, 19.25779914855957f } },
		{ "v_ilev_cs_door01",{ 1196.824951171875f, 2703.220947265625f, 38.3726f } },
		{ "v_ilev_cs_door01_r",{ 1199.1f, 2703.220947265625f, 38.3726f } },
		{ "v_ilev_cs_door01",{ -818.7642211914062f, -1079.54443359375f, 11.478099822998047f } },
		{ "v_ilev_cs_door01_r",{ -816.793212890625f, -1078.406494140625f, 11.478099822998047f } },
		{ "v_ilev_cs_door01",{ -0.0564f, 6517.4609375f, 32.027801513671875f } },
		{ "v_ilev_cs_door01_r",{ 1.7253f, 6515.91357421875f, 32.027801513671875f } },
		{ "v_ilev_clothmiddoor",{ -1201.4349365234375f, -776.8566284179688f, 17.9918f } },
		{ "v_ilev_clothmiddoor",{ 617.2457885742188f, 2751.022216796875f, 42.7578010559082f } },
		{ "v_ilev_clothmiddoor",{ 127.82f, -211.827392578125f, 55.2275f } },
		{ "v_ilev_clothmiddoor",{ -3167.75f, 1055.5357666015625f, 21.532899856567383f } },
		{ "v_ilev_ch_glassdoor",{ -716.6754150390625f, 155.42f, 37.6749f } },
		{ "v_ilev_ch_glassdoor",{ -715.6154174804688f, -157.256103515625f, 37.6749f } },
		{ "v_ilev_ch_glassdoor",{ -157.0924072265625f, -306.4413146972656f, 39.994f } },
		{ "v_ilev_ch_glassdoor",{ -156.40220642089844f, -304.4366149902344f, 39.994f } },
		{ "v_ilev_ch_glassdoor",{ -1454.7818603515625f, -231.79269409179688f, 50.05649948120117f } },
		{ "v_ilev_ch_glassdoor",{ -1456.2f, -233.36819458f, 50.05649948120117f } },
		{ "v_ilev_ta_door",{ 321.81f, 178.36f, 103.68f } },
		{ "v_ilev_ml_door1",{ 1859.89f, 3749.79f, 33.18f } },
		{ "v_ilev_ml_door1",{ -289.1752014160156f, 6199.1123046875f, 31.637f } },
		{ "v_ilev_ta_door",{ -1155.4541015625f, -1424.0079345703125f, 5.0461f } },
		{ "v_ilev_ta_door",{ 1321.28564453125f, -1650.5966796875f, 52.36629867553711f } },
		{ "v_ilev_ta_door",{ -3167.788818359375f, 1074.766845703125f, 20.9209f } },
		{ "v_ilev_mm_doorm_l",{ -817.0f, 179.0f, 73.0f } },
		{ "v_ilev_mm_doorm_r",{ -816.0f, 178.0f, 73.0f } },
		{ "prop_ld_garaged_01",{ -815.0f, 186.0f, 73.0f } },
		{ "prop_bh1_48_backdoor_l",{ -797.0f, 177.0f, 73.0f } },
		{ "prop_bh1_48_backdoor_r",{ -795.0f, 178.0f, 73.0f } },
		{ "prop_bh1_48_backdoor_l",{ -793.0f, 181.0f, 73.0f } },
		{ "prop_bh1_48_backdoor_r",{ -794.0f, 183.0f, 73.0f } },
		{ "prop_bh1_48_gate_1",{ -849.0f, 179.0f, 70.0f } },
		{ "v_ilev_mm_windowwc",{ -802.7332763671875f, 167.5041046142578f, 77.5823974609375f } },
		{ "v_ilev_fa_frontdoor",{ -14.0f, -1441.0f, 31.0f } },
		{ "v_ilev_fh_frntdoor",{ -15.0f, -1427.0f, 31.0f } },
		{ "prop_sc1_21_g_door_01",{ -25.28f, -1431.06f, 30.84f } },
		{ "v_ilev_fh_frontdoor",{ 7.52f, 539.53f, 176.17999999999998f } },
		{ "v_ilev_trevtraildr",{ 1973.0f, 3815.0f, 34.0f } },
		{ "prop_cs4_10_tr_gd_01",{ 1972.787353515625f, 3824.5537109375f, 32.583099365234375f } },
		{ "v_ilev_trev_doorfront",{ -1150.0f, -1521.0f, 11.0f } },
		{ "prop_com_ls_door_01",{ -1145.9f, -1991.14f, 14.18f } },
		{ "prop_id2_11_gdoor",{ 723.12f, -1088.8299560546875f, 23.28f } },
		{ "prop_com_ls_door_01",{ 356.09f, -134.77f, 40.01f } },
		{ "v_ilev_carmod3door",{ 108.8501968383789f, 6617.87646484375f, 32.673f } },
		{ "v_ilev_carmod3door",{ 114.32060241699219f, 6623.22607421875f, 32.7160987854f } },
		{ "v_ilev_carmod3door",{ 1182.305419921875f, 2645.2421875f, 38.806999999999995f } },
		{ "v_ilev_carmod3door",{ 1174.6541748046875f, 2645.2421875f, 38.68259811401367f } },
		{ "v_ilev_janitor_frontdoor",{ -107.5401f, -9.025799751281738f, 70.66960144042969f } },
		{ "v_ilev_ss_door8",{ 717.0f, -975.0f, 25.0f } },
		{ "v_ilev_ss_door7",{ 719.0f, -975.0f, 25.0f } },
		{ "v_ilev_ss_door02",{ 709.9813232421875f, -963.5311279296875f, 30.54529953f } },
		{ "v_ilev_ss_door03",{ 709.9893798828125f, -960.66748046875f, 30.54529953f } },
		{ "v_ilev_store_door",{ 707.8046264648438f, -962.4564208984375f, 30.54529953f } },
		{ "v_ilev_ml_door1",{ 1393.0f, 3599.0f, 35.0f } },
		{ "v_ilev_ml_door1",{ 1395.0f, 3600.0f, 35.0f } },
		{ "v_ilev_ss_door04",{ 1387.0f, 3614.0f, 39.0f } },
		{ "prop_ron_door_01",{ 1083.547119140625f, -1975.4354248046875f, 31.6222f } },
		{ "prop_ron_door_01",{ 1065.237060546875f, -2006.0791015625f, 32.2328987121582f } },
		{ "prop_ron_door_01",{ 1085.306884765625f, -2018.561279296875f, 41.62889862060547f } },
		{ "v_ilev_bank4door02",{ -111.0f, 6464.0f, 32.0f } },
		{ "v_ilev_bank4door01",{ -110.0f, 6462.0f, 32.0f } },
		{ "v_ilev_lester_doorfront",{ 1274.0f, -1721.0f, 55.0f } },
		{ "v_ilev_lester_doorveranda",{ 1271.89f, -1707.5699462890625f, 53.79f } },
		{ "v_ilev_lester_doorveranda",{ 1270.77f, 1708.1f, 53.75f } },
		{ "v_ilev_deviantfrontdoor",{ -127.5f, -1456.18f, 37.94f } },
		{ "prop_com_gar_door_01",{ 483.56f, -1316.0799560546875f, 32.18f } },
		{ "v_ilev_cs_door",{ 483.0f, -1312.0f, 29.0f } },
		{ "prop_strip_door_01",{ 128.0f, -1299.0f, 29.0f } },
		{ "prop_magenta_door",{ 96.0f, -1285.0f, 29.0f } },
		{ "prop_motel_door_09",{ 549.0f, -1773.0f, 34.0f } },
		{ "v_ilev_gangsafedoor",{ 974.0f, -1839.0f, 36.0f } },
		{ "v_ilev_gangsafedoor",{ 977.0f, -105.0f, 75.0f } },
		{ "v_ilev_ra_door1_l",{ 1391.0f, 1163.0f, 114.0f } },
		{ "v_ilev_ra_door1_r",{ 1391.0f, 1161.0f, 114.0f } },
		{ "prop_cs6_03_door_l",{ 1396.0f, 1143.0f, 115.0f } },
		{ "prop_cs6_03_door_r",{ 1396.0f, 1141.0f, 115.0f } },
		{ "v_ilev_ra_door1_l",{ 1409.0f, 1146.0f, 114.0f } },
		{ "v_ilev_ra_door1_r",{ 1409.0f, 1148.0f, 114.0f } },
		{ "v_ilev_ra_door1_l",{ 1408.0f, 1159.0f, 114.0f } },
		{ "v_ilev_ra_door1_r",{ 1408.0f, 1161.0f, 114.0f } },
		{ "prop_gar_door_01",{ -1067.0f, -1666.0f, 5.0f } },
		{ "prop_gar_door_02",{ -1065.0f, -1669.0f, 5.0f } },
		{ "prop_map_door_01",{ -1104.66f, -1638.47998046875f, 4.68f } },
		{ "v_ilev_fib_door1",{ 31.72f, -1101.8499755859375f, 26.57f } },
		{ "v_ilev_tort_door",{ 134.39999999999998f, -2204.1f, 7.52f } },
		{ "v_ilev_bl_shutter2",{ 3628.0f, 3747.0f, 28.0f } },
		{ "v_ilev_bl_shutter2",{ 3621.0f, 3752.0f, 28.0f } },
		{ "v_ilev_rc_door3_l",{ -608.72998046875f, -1610.3199462890625f, 27.16f } },
		{ "v_ilev_rc_door3_r",{ -611.32f, -1610.0899658203125f, 27.16f } },
		{ "v_ilev_rc_door3_l",{ -592.94f, -1631.5799560546875f, 27.16f } },
		{ "v_ilev_rc_door3_r",{ -592.71f, 1628.99f, 27.16f } },
		{ "v_ilev_ss_door04",{ 1991.0f, 3053.0f, 47.0f } },
		{ "v_ilev_fh_door4",{ 1988.3529052734375f, 3054.410888671875f, 47.3204f } },
		{ "prop_epsilon_door_l",{ -700.1699829101562f, 47.31f, 44.3f } },
		{ "prop_epsilon_door_r",{ -697.94f, 48.35f, 44.3f } },
		{ "v_ilev_epsstoredoor",{ 241.35740661621094f, 361.0487976074219f, 105.89630126953125f } },
		{ "prop_ch2_09c_garage_door",{ -689.1099853515625f, 506.97f, 110.64f } },
		{ "v_ilev_door_orangesolid",{ -1055.9599609375f, 236.42999999999998f, 44.169999999999995f } },
		{ "prop_magenta_door",{ 29.0f, 3661.0f, 41.0f } },
		{ "prop_cs4_05_tdoor",{ 32.0f, 3667.0f, 41.0f } },
		{ "v_ilev_housedoor1",{ 87.0f, -1959.0f, 21.0f } },
		{ "v_ilev_fh_frntdoor",{ 0.0f, -1823.0f, 30.0f } },
		{ "p_cut_door_03",{ 23.34f, 1897.6f, 23.05f } },
		{ "p_cut_door_02",{ 524.2f, 3081.139892578125f, 41.16f } },
		{ "v_ilev_po_door",{ -1910.5799560546875f, -576.01f, 19.25f } },
		{ "prop_ss1_10_door_l",{ -720.39f, 256.8599853515625f, 80.29f } },
		{ "prop_ss1_10_door_r",{ -718.4199829101562f, 257.79f, 80.29f } },
		{ "v_ilev_fibl_door02",{ 106.38f, -742.7f, 46.18f } },
		{ "v_ilev_fibl_door01",{ 105.76f, -746.65f, 46.18f } },
		{ "v_ilev_ct_door01",{ -2343.53f, 3265.3701171875f, 32.96f } },
		{ "v_ilev_ct_door01",{ -2342.22998046875f, 3267.6201171875f, 32.96f } },
		{ "ap1_02_door_l",{ -1041.9326171875f, -2748.16748046875f, 22.030799865722656f } },
		{ "ap1_02_door_r",{ -1044.8408203125f, -2746.48876953125f, 22.030799865722656f } },
		{ "v_ilev_fb_doorshortl",{ -1045.119873046875f, -232.00399780273438f, 39.4379f } },
		{ "v_ilev_fb_doorshortr",{ -1046.51611328125f, -229.35809326171875f, 39.4379f } },
		{ "v_ilev_fb_door01",{ -1083.6201171875f, -260.41668701171875f, 38.18669891357422f } },
		{ "v_ilev_fb_door02",{ -1080.974365234375f, -259.0203857421875f, 38.18669891357422f } },
		{ "v_ilev_gtdoor",{ -1042.5699462890625f, -240.6f, 38.11f } },
		{ "prop_damdoor_01",{ 1385.2578125f, -2079.949462890625f, 52.76380157470703f } },
		{ "v_ilev_genbankdoor2",{ 1656.5699462890625f, 4849.66015625f, 42.35f } },
		{ "v_ilev_genbankdoor1",{ 1656.25f, 4852.240234375f, 42.35f } },
		{ "prop_sec_barrier_ld_01a",{ -1051.40185546875f, -474.6846923828125f, 36.61989974975586f } },
		{ "prop_sec_barrier_ld_01a",{ -1049.2852783203125f, -476.6376037597656f, 36.7584f } },
		{ "prop_sec_barrier_ld_02a",{ -1210.9566650390625f, -580.8765258789062f, 27.2373f } },
		{ "prop_sec_barrier_ld_02a",{ -1212.4453125f, -578.4401245117188f, 27.2373f } },
		{ "v_ilev_roc_door4",{ -565.1712036132812f, 276.625885f, 83.2863f } },
		{ "v_ilev_roc_door4",{ -561.2863159179688f, 293.5043029785156f, 87.777099609375f } },
		{ "p_jewel_door_l",{ -631.96f, -236.33f, 38.21f } },
		{ "p_jewel_door_r1",{ 630.43f, -238.44f, 38.21f } },
		{ "prop_ld_bankdoors_01",{ 231.62f, 216.23f, 106.4f } },
		{ "prop_ld_bankdoors_01",{ 232.72f, 213.88f, 106.4f } },
		{ "hei_prop_hei_bankdoor_new",{ 258.32f, 203.84f, 106.43f } },
		{ "hei_prop_hei_bankdoor_new",{ 260.76f, 202.95f, 106.43f } },
		{ "hei_v_ilev_bk_gate_pris",{ 256.31f, 220.66f, 106.43f } },
		{ "v_ilev_bk_door",{ 266.3599853515625f, 217.57f, 110.43f } },
		{ "v_ilev_shrf2door",{ -442.66f, 6015.2216796875f, 31.8663f } },
		{ "v_ilev_shrf2door",{ -444.4985046386719f, 6017.06f, 31.8663f } },
		{ "v_ilev_shrfdoor",{ 1855.684814453125f, 3683.93017578125f, 34.5928f } },
		{ "prop_bhhotel_door_l",{ -1223.3499755859375f, -172.41f, 39.98f } },
		{ "prop_bhhotel_door_r",{ -1220.93f, 173.67999999999998f, 39.98f } },
		{ "prop_bhhotel_door_l",{ 1211.99f, -190.57f, 39.98f } },
		{ "prop_bhhotel_door_r",{ -1213.26f, 192.98f, 39.98f } },
		{ "prop_bhhotel_door_l",{ -1217.77f, 201.54f, 39.98f } },
		{ "prop_bhhotel_door_r",{ -1219.04f, 203.95f, 39.98f } },
		{ "prop_ch3_04_door_01l",{ 2514.32f, 317.34f, 93.32f } },
		{ "prop_ch3_04_door_01r",{ 2512.419921875f, -319.26f, 93.32f } },
		{ "prop_ch3_01_trlrdoor_l",{ 2333.22998046875f, 2574.969970703125f, 47.03f } },
		{ "prop_ch3_01_trlrdoor_r",{ 2329.64990234375f, 2576.639892578125f, 47.03f } },
		{ "v_ilev_gc_door04",{ 16.127899169921875f, -1114.60546875f, 29.9468994140625f } },
		{ "v_ilev_gc_door03",{ 18.57200050354f, -1115.4951171875f, 29.9468994140625f } },
		{ "v_ilev_gc_door04",{ 1698.17626953125f, 3751.505615234375f, 34.8553f } },
		{ "v_ilev_gc_door03",{ 1699.9371337890625f, 3753.420166015625f, 34.8553f } },
		{ "v_ilev_gc_door04",{ 244.72740173339844f, -44.0791015625f, 70.91f } },
		{ "v_ilev_gc_door03",{ 243.83790588378906f, -46.5232f, 70.91f } },
		{ "v_ilev_gc_door04",{ 845.3624267578125f, -1024.5390625f, 28.3448f } },
		{ "v_ilev_gc_door03",{ 842.7683715820312f, -1024.5390625f, 23.3448f } },
		{ "v_ilev_gc_door04",{ -326.1122131347656f, 6075.26953125f, 31.6047000885f } },
		{ "v_ilev_gc_door03",{ -324.27301025390625f, 6077.10888671875f, 31.6047000885f } },
		{ "v_ilev_gc_door04",{ -665.2423706054688f, -944.3256225585938f, 21.9792f } },
		{ "v_ilev_gc_door03",{ -662.6414184570312f, -944.3256225585938f, 21.9792f } },
		{ "v_ilev_gc_door04",{ -1313.825927734375f, -389.125885f, 36.845699310302734f } },
		{ "v_ilev_gc_door03",{ -1314.4649658203125f, -391.6471862792969f, 36.845699310302734f } },
		{ "v_ilev_gc_door04",{ -1114.0089111328125f, 2689.77f, 18.704099655151367f } },
		{ "v_ilev_gc_door03",{ -1112.0708f, 2691.504638671875f, 18.704099655151367f } },
		{ "v_ilev_gc_door04",{ -3164.84521484375f, 1081.3917236328125f, 20.9887f } },
		{ "v_ilev_gc_door03",{ -3163.8115234375f, 1083.7784423828125f, 20.9887f } },
		{ "v_ilev_gc_door04",{ 2570.904541015625f, 303.3555908203125f, 108.88480377197266f } },
		{ "v_ilev_gc_door03",{ 2568.3037109375f, 303.3555908203125f, 108.88480377197266f } },
		{ "v_ilev_gc_door04",{ 813.1779174804688f, -2148.26953125f, 29.7689f } },
		{ "v_ilev_gc_door03",{ 810.576904296875f, -2148.26953125f, 29.7689f } },
		{ "v_ilev_gc_door01",{ 6.8179f, -1098.20947265625f, 29.9468994140625f } },
		{ "v_ilev_gc_door01",{ 827.5341796875f, -2160.49267578125f, 29.7688f } },
		{ "prop_lrggate_01c_l",{ -1107.01f, 289.38f, 64.76f } },
		{ "prop_lrggate_01c_r",{ 1101.62f, 290.3599853515625f, 64.76f } },
		{ "prop_lrggate_01c_l",{ -1138.64f, 300.82f, 67.18f } },
		{ "prop_lrggate_01c_r",{ -1137.05f, 295.59f, 67.18f } },
		{ "v_ilev_bl_doorel_l",{ -2053.159912109375f, 3239.49f, 30.5f } },
		{ "v_ilev_bl_doorel_r",{ -2054.389892578125f, 3237.22998046875f, 30.5f } },
		{ "v_ilev_cbankcountdoor01",{ -108.91f, 6469.10986328125f, 31.91f } },
		{ "prop_fnclink_03gate5",{ -182.91f, 6168.3701171875f, 32.14f } },
		{ "v_ilev_csr_door_l",{ 59.89f, -1092.949951171875f, 26.88f } },
		{ "v_ilev_csr_door_r",{ 60.55f, -1094.75f, 26.89f } },
		{ "v_ilev_csr_door_l",{ -39.13f, -1108.219970703125f, 26.72f } },
		{ "v_ilev_csr_door_r",{ -37.33f, 1108.87f, 26.72f } },
		{ "prop_ron_door_01",{ 1943.72998046875f, 3803.6298828125f, 32.31f } },
		{ "v_ilev_genbankdoor2",{ 316.39f, 276.49f, 54.52f } },
		{ "v_ilev_genbankdoor1",{ 313.96f, -275.6f, 54.52f } },
		{ "v_ilev_genbankdoor2",{ -2965.7099609375f, 484.22f, 16.05f } },
		{ "v_ilev_genbankdoor1",{ -2965.82f, 481.63f, 16.05f } },
		{ "v_ilev_abbmaindoor",{ 962.1f, -2183.83f, 31.06f } },
		{ "v_ilev_abbmaindoor2",{ 961.7899780273438f, -2187.08f, 31.06f } },
		{ "prop_ch3_04_door_02",{ 2508.429931640625f, -336.63f, 115.76f } },
		{ "prop_ch1_07_door_01l",{ -2255.18994140625f, 322.26f, 184.92999999999998f } },
		{ "prop_ch1_07_door_01r",{ -2254.06f, 319.7f, 184.92999999999998f } },
		{ "prop_ch1_07_door_01l",{ -2301.1298828125f, 336.91f, 184.92999999999998f } },
		{ "prop_ch1_07_door_01r",{ -2298.57f, 338.04998779296875f, 184.92999999999998f } },
		{ "prop_ch1_07_door_01l",{ -2222.32f, 305.8599853515625f, 184.92999999999998f } },
		{ "prop_ch1_07_door_01r",{ -2221.18994140625f, 303.3f, 184.92999999999998f } },
		{ "prop_ch1_07_door_01l",{ -2280.6f, 265.43f, 184.92999999999998f } },
		{ "prop_ch1_07_door_01r",{ -2278.04f, 266.57f, 184.92999999999998f } },
		{ "prop_gar_door_04",{ 778.31f, 1867.49f, 30.66f } },
		{ "prop_gate_tep_01_l",{ -721.3499755859375f, 91.01f, 56.68f } },
		{ "prop_gate_tep_01_r",{ -728.84f, 88.64f, 56.68f } },
		{ "prop_artgallery_02_dr",{ -2287.6201171875f, 363.9f, 174.92999999999998f } },
		{ "prop_artgallery_02_dl",{ -2289.78f, 362.91f, 174.92999999999998f } },
		{ "prop_artgallery_02_dr",{ -2289.860107421875f, 362.88f, 174.92999999999998f } },
		{ "prop_artgallery_02_dl",{ -2292.01f, 361.89f, 174.92999999999998f } },
		{ "prop_fnclink_07gate1",{ 1803.93994140625f, 3929.01f, 33.72f } },
		{ "v_ilev_genbankdoor2",{ 348.81f, 47.26f, 49.39f } },
		{ "v_ilev_genbankdoor1",{ -351.26f, 46.41f, 49.39f } },
		{ "prop_abat_slide",{ 962.9083862304688f, -2105.813720703125f, 34.6432f } }
} };
#endif