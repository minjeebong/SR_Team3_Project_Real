#ifndef Engine_Enum_h__
#define Engine_Enum_h__

namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };

	// Dynamic 컴포넌트 경우 매 프레임마다 갱신해야하는 컴포넌트 집단
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO {	INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

	enum TEXTUREID { TEX_NORMAL, TEX_CUBE, TEX_END };

	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE {	DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	// 09.19 안주헌
	enum PIPEFLOW { FLOW_UP, FLOW_DOWN, FLOW_LEFT, FLOW_RIGHT, FLOW_END };

	enum CHANNELID
	{
		SOUND_BGM,
		SOUND_EFFECT,
		SOUND_SURROUNDING,
		MAXCHANNEL
	};

	//0927 봉민지
	enum TEXTRANGE { TEXT_DEFAULT, TEXT_CENTER, TEXTRANGE_END };

	// 09.24 김선환
	enum GROUP_TYPE { PLAYER, MONSTER, OBJECT, NPC, GROUP_END = 32 };


	// 09.28 동영
	enum OBJ_TYPE { PUSH_ABLE, LIFT_ABLE, DESTROY_ABLE, NONE, TYPE_END };

	// 행동 순서는 변경해도 되나, 무조건 DIAGONAL은 이전 행동 다음에 적을 것
	enum PLAYERSTATE
	{
		PLY_IDLE,
		PLY_IDLEDIAGONAL,
		PLY_MOVE,
		PLY_MOVEDIAGONAL,
		PLY_DASH,
		PLY_DASHDIAGONAL,
		PLY_SWING,
		PLY_SWINGDIAGONAL,
		PLY_IDLEDANCE,
		PLY_PUSH,
		PLY_LIFTSTART,
		PLY_LIFTIDLE,
		PLY_LIFTMOVE,
		PLY_LIFTMOVEDIAGONAL,
		PLY_LIFTEND,
		PLY_END
	};

	// 숫자 변경하지 말 것
	enum OBJ_DIRECTION
	{
		OBJDIR_FRONT = 1,
		OBJDIR_BACK = 2,
		OBJDIR_LEFT = 4,
		OBJDIR_RIGHT = 8,

		OBJDIR_LEFTFRONT = 5,
		OBJDIR_LEFTBACK = 6,
		OBJDIR_RIGHTFRONT = 9,
		OBJDIR_RIGHTBACK = 10,

		OBJDIR_NONE = 0,
		OBJDIR_END
	};
}
#endif // Engine_Enum_h__
