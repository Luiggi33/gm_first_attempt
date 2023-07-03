#include <GarrysMod/Lua/Interface.h>

#include <GarrysMod/InterfacePointers.hpp>

#include <iserver.h>
#include <iclient.h>

LUA_FUNCTION( CalculateVectorMiddlePoint )
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Vector);
	LUA->CheckType(2, GarrysMod::Lua::Type::Vector);

	Vector vec1 = (LUA->GetVector(1));
	Vector vec2 = (LUA->GetVector(2));

	Vector middlePoint = Vector();
	middlePoint.x = (vec1.x + vec2.x) / 2;
	middlePoint.y = (vec1.y + vec2.y) / 2;
	middlePoint.z = (vec1.z + vec2.z) / 2;

	LUA->PushVector(middlePoint);

	return 1;
}

LUA_FUNCTION( ReconnectPlayer )
{
	LUA->CheckType(1, GarrysMod::Lua::Type::Number);

	int client_id = (LUA->GetNumber(1));
	IServer* server = InterfacePointers::Server();
	
	if (server == nullptr) {
		LUA->ThrowError("invalid server");
	}

	IClient* player = server->GetClient(client_id);

	if (player == nullptr) {
		LUA->ThrowError("invalid player");
	}

	if (!player->IsFakeClient()) {
		LUA->PushBool(false);
	} else {
		player->Reconnect();
		LUA->PushBool(true);
	}

	return 1;
}

GMOD_MODULE_OPEN()
{
	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->GetField(-1, "util");
		LUA->PushCFunction(CalculateVectorMiddlePoint);
		LUA->SetField(-2, "CalculateVectorMiddlePoint");
	LUA->Pop();

	LUA->PushSpecial(GarrysMod::Lua::SPECIAL_GLOB);
		LUA->PushCFunction(ReconnectPlayer);
		LUA->SetField(-2, "ReconnectPlayer");
	LUA->Pop();

	return 0;
}

GMOD_MODULE_CLOSE()
{
	return 0;
}