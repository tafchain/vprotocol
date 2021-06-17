#ifndef VBH_COMMITTER_ROUTER_H_4CA973A4879311E9875660F18A3A20D1
#define VBH_COMMITTER_ROUTER_H_4CA973A4879311E9875660F18A3A20D1

#include "dsc/dsc_msg.h"
#include "dsc/configure/dsc_configure.h"
#include "common/vbh_comm_msg_def.h"


namespace VBH
{
	//x committer sevice 的路由器
	class VBH_COMM_DEF_EXPORT CXCommitterServiceRouter
	{
	private:
		struct SDscAddr //在DSC系统中 地址 的 ID部分
		{
			ACE_INT16 m_nNodeID;
			ACE_INT16 m_nServiceID;
		};

	public:
		//初始化
		ACE_INT32 Init();

		//获取特定channel 用户的 xcs地址
		ACE_INT32 GetXcsAddr(CDscMsg::CDscMsgAddr& addr, const ACE_UINT32 nChannelID);

	private:
		//加载 channel 和 tcs addr 的映射关系
		ACE_INT32 LoadChannelMapXcsAddr();

	private:
		using channel_map_dsc_addr_type = dsc_unordered_map_type(ACE_UINT32, SDscAddr); //channel -> xcs addr

	private:
		channel_map_dsc_addr_type m_mapChannelMapXcsAddr;
	};

}

#include "common/vbh_committer_router.inl"


#endif // VBH_COMMITER_PARAMETER_H
