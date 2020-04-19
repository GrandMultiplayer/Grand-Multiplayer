#ifndef __RPC_MANAGER_H__
#define __RPC_MANAGER_H__
namespace RPC
{
	namespace Manager
	{
		void RegisterRPCMessages();
		void UnregisterRPCMessages();


		typedef struct
		{
			const char *name;
			void(*functionPointer) (RakNet::BitStream *userData, RakNet::Packet *packet);
		} RPCMessage;
	}
}
#endif