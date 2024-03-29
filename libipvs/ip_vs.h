/*
 *      IP Virtual Server
 *      data structure and functionality definitions
 */

#ifndef _IP_VS_H
#define _IP_VS_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/types.h>	/* For __beXX types in userland */

#ifdef LIBIPVS_USE_NL
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#endif

#define IP_VS_VERSION_CODE	0x010201
#define NVERSION(version)			\
	(version >> 16) & 0xFF,			\
	(version >> 8) & 0xFF,			\
	version & 0xFF

/*
 *      Virtual Service Flags
 */
#define IP_VS_SVC_F_PERSISTENT	0x0001		/* persistent port */
#define IP_VS_SVC_F_HASHED	0x0002		/* hashed entry */
#define IP_VS_SVC_F_ONEPACKET	0x0004		/* one-packet scheduling */

/*
 *      IPVS sync daemon states
 */
#define IP_VS_STATE_NONE	0x0000		/* daemon is stopped */
#define IP_VS_STATE_MASTER	0x0001		/* started as master */
#define IP_VS_STATE_BACKUP	0x0002		/* started as backup */

/*
 *      IPVS socket options
 */
#define IP_VS_BASE_CTL		(64+1024+64)		/* base */

#define IP_VS_SO_SET_NONE	IP_VS_BASE_CTL		/* just peek */
#define IP_VS_SO_SET_INSERT	(IP_VS_BASE_CTL+1)
#define IP_VS_SO_SET_ADD	(IP_VS_BASE_CTL+2)
#define IP_VS_SO_SET_EDIT	(IP_VS_BASE_CTL+3)
#define IP_VS_SO_SET_DEL	(IP_VS_BASE_CTL+4)
#define IP_VS_SO_SET_FLUSH	(IP_VS_BASE_CTL+5)
#define IP_VS_SO_SET_LIST	(IP_VS_BASE_CTL+6)
#define IP_VS_SO_SET_ADDDEST	(IP_VS_BASE_CTL+7)
#define IP_VS_SO_SET_DELDEST	(IP_VS_BASE_CTL+8)
#define IP_VS_SO_SET_EDITDEST	(IP_VS_BASE_CTL+9)
#define IP_VS_SO_SET_TIMEOUT	(IP_VS_BASE_CTL+10)
#define IP_VS_SO_SET_STARTDAEMON (IP_VS_BASE_CTL+11)
#define IP_VS_SO_SET_STOPDAEMON (IP_VS_BASE_CTL+12)
#define IP_VS_SO_SET_RESTORE    (IP_VS_BASE_CTL+13)
#define IP_VS_SO_SET_SAVE       (IP_VS_BASE_CTL+14)
#define IP_VS_SO_SET_ZERO	(IP_VS_BASE_CTL+15)
#define IP_VS_SO_SET_MAX	IP_VS_SO_SET_ZERO

#define IP_VS_SO_GET_VERSION	IP_VS_BASE_CTL
#define IP_VS_SO_GET_INFO	(IP_VS_BASE_CTL+1)
#define IP_VS_SO_GET_SERVICES	(IP_VS_BASE_CTL+2)
#define IP_VS_SO_GET_SERVICE	(IP_VS_BASE_CTL+3)
#define IP_VS_SO_GET_DESTS	(IP_VS_BASE_CTL+4)
#define IP_VS_SO_GET_DEST	(IP_VS_BASE_CTL+5)	/* not used now */
#define IP_VS_SO_GET_TIMEOUT	(IP_VS_BASE_CTL+6)
#define IP_VS_SO_GET_DAEMON	(IP_VS_BASE_CTL+7)
#define IP_VS_SO_GET_MAX	IP_VS_SO_GET_DAEMON


/*
 *      IPVS Connection Flags
 */
#define IP_VS_CONN_F_FWD_MASK	0x0007		/* mask for the fwd methods */
#define IP_VS_CONN_F_MASQ	0x0000		/* masquerading/NAT */
#define IP_VS_CONN_F_LOCALNODE	0x0001		/* local node */
#define IP_VS_CONN_F_TUNNEL	0x0002		/* tunneling */
//dr转发方式
#define IP_VS_CONN_F_DROUTE	0x0003		/* direct routing */
#define IP_VS_CONN_F_BYPASS	0x0004		/* cache bypass */
#define IP_VS_CONN_F_SYNC	0x0020		/* entry created by sync */
#define IP_VS_CONN_F_HASHED	0x0040		/* hashed entry */
#define IP_VS_CONN_F_NOOUTPUT	0x0080		/* no output packets */
#define IP_VS_CONN_F_INACTIVE	0x0100		/* not established */
#define IP_VS_CONN_F_OUT_SEQ	0x0200		/* must do output seq adjust */
#define IP_VS_CONN_F_IN_SEQ	0x0400		/* must do input seq adjust */
#define IP_VS_CONN_F_SEQ_MASK	0x0600		/* in/out sequence mask */
#define IP_VS_CONN_F_NO_CPORT	0x0800		/* no client port set yet */
#define IP_VS_CONN_F_TEMPLATE	0x1000		/* template, not connection */
#define IP_VS_CONN_F_ONE_PACKET	0x2000		/* forward only one packet */

#define IP_VS_SCHEDNAME_MAXLEN	16
#define IP_VS_PENAME_MAXLEN	16
#define IP_VS_IFNAME_MAXLEN	16

#define IP_VS_PEDATA_MAXLEN	255

union nf_inet_addr {
        __u32           all[4];
        __be32          ip;
        __be32          ip6[4];
        struct in_addr  in;
        struct in6_addr in6;
};

/*
 *	The struct ip_vs_service_user and struct ip_vs_dest_user are
 *	used to set IPVS rules through setsockopt.
 */
struct ip_vs_service_kern {
	/* virtual service addresses */
	u_int16_t		protocol;
	__be32			addr;	/* virtual ip address */
	__be16			port;
	u_int32_t		fwmark;		/* firwall mark of service */

	/* virtual service options */
	char			sched_name[IP_VS_SCHEDNAME_MAXLEN];
	unsigned		flags;		/* virtual service flags */
	unsigned		timeout;	/* persistent timeout in sec */
	__be32			netmask;	/* persistent netmask */
};

struct ip_vs_service_user {
	/* virtual service addresses */
	u_int16_t		protocol;
	__be32			__addr_v4;	/* virtual ip address - internal use only */
	__be16			port;
	u_int32_t		fwmark;		/* firwall mark of service */

	/* virtual service options */
	//调度器名称
	char			sched_name[IP_VS_SCHEDNAME_MAXLEN];
	unsigned		flags;		/* virtual service flags */
	unsigned		timeout;	/* persistent timeout in sec */
	__be32			netmask;	/* persistent netmask */
	u_int16_t		af;//协议族
	union nf_inet_addr	addr;
	char			pe_name[IP_VS_PENAME_MAXLEN];
};

struct ip_vs_dest_kern {
	/* destination server address */
	__be32			addr;
	__be16			port;

	/* real server options */
	unsigned		conn_flags;	/* connection flags */
	int			weight;		/* destination weight */

	/* thresholds for active connections */
	u_int32_t		u_threshold;	/* upper threshold */
	u_int32_t		l_threshold;	/* lower threshold */
};

struct ip_vs_dest_user {
	/* destination server address */
	__be32			__addr_v4; /* internal use only */
	__be16			port;

	/* real server options */
	unsigned		conn_flags;	/* connection flags */
	int			weight;		/* destination weight */

	/* thresholds for active connections */
	u_int32_t		u_threshold;	/* upper threshold */
	u_int32_t		l_threshold;	/* lower threshold */
	u_int16_t		af;
	union nf_inet_addr	addr;
};

/*
 *	IPVS statistics object (for user space)
 */
struct ip_vs_stats_user
{
	__u32                   conns;          /* connections scheduled */
	__u32                   inpkts;         /* incoming packets */
	__u32                   outpkts;        /* outgoing packets */
	__u64                   inbytes;        /* incoming bytes */
	__u64                   outbytes;       /* outgoing bytes */

	__u32			cps;		/* current connection rate */
	__u32			inpps;		/* current in packet rate */
	__u32			outpps;		/* current out packet rate */
	__u32			inbps;		/* current in byte rate */
	__u32			outbps;		/* current out byte rate */
};


/* The argument to IP_VS_SO_GET_INFO */
struct ip_vs_getinfo {
	/* version number */
	unsigned int		version;

	/* size of connection hash table */
	unsigned int		size;

	/* number of virtual services */
	unsigned int		num_services;
};


/* The argument to IP_VS_SO_GET_SERVICE */
struct ip_vs_service_entry_kern {
	/* which service: user fills in these */
	u_int16_t		protocol;
	__be32			addr;	/* virtual address */
	__be16			port;
	u_int32_t		fwmark;		/* firwall mark of service */

	/* service options */
	char			sched_name[IP_VS_SCHEDNAME_MAXLEN];
	unsigned		flags;          /* virtual service flags */
	unsigned		timeout;	/* persistent timeout */
	__be32			netmask;	/* persistent netmask */

	/* number of real servers */
	unsigned int		num_dests;

	/* statistics */
	struct ip_vs_stats_user stats;
};

struct ip_vs_service_entry {
	/* which service: user fills in these */
	u_int16_t		protocol;
	__be32			__addr_v4;	/* virtual address - internal use only*/
	__be16			port;
	u_int32_t		fwmark;		/* firwall mark of service */

	/* service options */
	char			sched_name[IP_VS_SCHEDNAME_MAXLEN];
	unsigned		flags;          /* virtual service flags */
	unsigned		timeout;	/* persistent timeout */
	__be32			netmask;	/* persistent netmask */

	/* number of real servers */
	unsigned int		num_dests;

	/* statistics */
	struct ip_vs_stats_user stats;

	u_int16_t		af;
	union nf_inet_addr	addr;
	char			pe_name[IP_VS_PENAME_MAXLEN];

};

struct ip_vs_dest_entry_kern {
	__be32			addr;	/* destination address */
	__be16			port;
	unsigned		conn_flags;	/* connection flags */
	int			weight;		/* destination weight */

	u_int32_t		u_threshold;	/* upper threshold */
	u_int32_t		l_threshold;	/* lower threshold */

	u_int32_t		activeconns;	/* active connections */
	u_int32_t		inactconns;	/* inactive connections */
	u_int32_t		persistconns;	/* persistent connections */

	/* statistics */
	struct ip_vs_stats_user stats;
};

struct ip_vs_dest_entry {
	__be32			__addr_v4;	/* destination address - internal use only */
	__be16			port;
	unsigned		conn_flags;	/* connection flags */
	int			weight;		/* destination weight */

	u_int32_t		u_threshold;	/* upper threshold */
	u_int32_t		l_threshold;	/* lower threshold */

	u_int32_t		activeconns;	/* active connections */
	u_int32_t		inactconns;	/* inactive connections */
	u_int32_t		persistconns;	/* persistent connections */

	/* statistics */
	struct ip_vs_stats_user stats;
	u_int16_t		af;
	union nf_inet_addr	addr;
};

/* The argument to IP_VS_SO_GET_DESTS */
struct ip_vs_get_dests_kern {
	/* which service: user fills in these */
	u_int16_t		protocol;
	__be32			addr;	/* virtual address - internal use only */
	__be16			port;
	u_int32_t		fwmark;		/* firwall mark of service */

	/* number of real servers */
	unsigned int		num_dests;

	/* the real servers */
	struct ip_vs_dest_entry_kern	entrytable[0];
};

struct ip_vs_get_dests {
	/* which service: user fills in these */
	u_int16_t		protocol;
	__be32			__addr_v4;	/* virtual address - internal use only */
	__be16			port;
	u_int32_t		fwmark;		/* firwall mark of service */

	/* number of real servers */
	unsigned int		num_dests;
	u_int16_t		af;
	union nf_inet_addr	addr;

	/* the real servers */
	struct ip_vs_dest_entry	entrytable[0];
};

/* The argument to IP_VS_SO_GET_SERVICES */
struct ip_vs_get_services {
	/* number of virtual services */
	unsigned int		num_services;

	/* service table */
	struct ip_vs_service_entry entrytable[0];
};

struct ip_vs_get_services_kern {
	/* number of virtual services */
	unsigned int		num_services;

	/* service table */
	struct ip_vs_service_entry_kern entrytable[0];
};

/* The argument to IP_VS_SO_GET_TIMEOUT */
struct ip_vs_timeout_user {
	int			tcp_timeout;
	int			tcp_fin_timeout;
	int			udp_timeout;
};


/* The argument to IP_VS_SO_GET_DAEMON */
struct ip_vs_daemon_user {
	/* sync daemon state (master/backup) */
	int			state;

	/* multicast interface name */
	char			mcast_ifn[IP_VS_IFNAME_MAXLEN];

	/* SyncID we belong to */
	int			syncid;
};


/*
 *
 * IPVS Generic Netlink interface definitions
 *
 */

/* Generic Netlink family info */

#define IPVS_GENL_NAME		"IPVS"
#define IPVS_GENL_VERSION	0x1

struct ip_vs_flags {
	__be32 flags;
	__be32 mask;
};

/* Generic Netlink command attributes */
enum {
	IPVS_CMD_UNSPEC = 0,

	IPVS_CMD_NEW_SERVICE,		/* add service */
	IPVS_CMD_SET_SERVICE,		/* modify service */
	IPVS_CMD_DEL_SERVICE,		/* delete service */
	IPVS_CMD_GET_SERVICE,		/* get info about specific service */

	IPVS_CMD_NEW_DEST,		/* add destination */
	IPVS_CMD_SET_DEST,		/* modify destination */
	IPVS_CMD_DEL_DEST,		/* delete destination */
	IPVS_CMD_GET_DEST,		/* get list of all service dests */

	IPVS_CMD_NEW_DAEMON,		/* start sync daemon */
	IPVS_CMD_DEL_DAEMON,		/* stop sync daemon */
	IPVS_CMD_GET_DAEMON,		/* get sync daemon status */

	IPVS_CMD_SET_TIMEOUT,		/* set TCP and UDP timeouts */
	IPVS_CMD_GET_TIMEOUT,		/* get TCP and UDP timeouts */

	IPVS_CMD_SET_INFO,		/* only used in GET_INFO reply */
	IPVS_CMD_GET_INFO,		/* get general IPVS info */

	IPVS_CMD_ZERO,			/* zero all counters and stats */
	IPVS_CMD_FLUSH,			/* flush services and dests */

	__IPVS_CMD_MAX,
};

#define IPVS_CMD_MAX (__IPVS_CMD_MAX - 1)

/* Attributes used in the first level of commands */
enum {
	IPVS_CMD_ATTR_UNSPEC = 0,
	IPVS_CMD_ATTR_SERVICE,		/* nested service attribute */
	IPVS_CMD_ATTR_DEST,		/* nested destination attribute */
	IPVS_CMD_ATTR_DAEMON,		/* nested sync daemon attribute */
	IPVS_CMD_ATTR_TIMEOUT_TCP,	/* TCP connection timeout */
	IPVS_CMD_ATTR_TIMEOUT_TCP_FIN,	/* TCP FIN wait timeout */
	IPVS_CMD_ATTR_TIMEOUT_UDP,	/* UDP timeout */
	__IPVS_CMD_ATTR_MAX,
};

#define IPVS_CMD_ATTR_MAX (__IPVS_SVC_ATTR_MAX - 1)

/*
 * Attributes used to describe a service
 *
 * Used inside nested attribute IPVS_CMD_ATTR_SERVICE
 */
enum {
	IPVS_SVC_ATTR_UNSPEC = 0,
	IPVS_SVC_ATTR_AF,		/* address family */
	IPVS_SVC_ATTR_PROTOCOL,		/* virtual service protocol */
	IPVS_SVC_ATTR_ADDR,		/* virtual service address */
	IPVS_SVC_ATTR_PORT,		/* virtual service port */
	IPVS_SVC_ATTR_FWMARK,		/* firewall mark of service */

	IPVS_SVC_ATTR_SCHED_NAME,	/* name of scheduler */
	IPVS_SVC_ATTR_FLAGS,		/* virtual service flags */
	IPVS_SVC_ATTR_TIMEOUT,		/* persistent timeout */
	IPVS_SVC_ATTR_NETMASK,		/* persistent netmask */

	IPVS_SVC_ATTR_STATS,		/* nested attribute for service stats */

	IPVS_SVC_ATTR_PE_NAME,		/* name of scheduler */

	__IPVS_SVC_ATTR_MAX,
};

#define IPVS_SVC_ATTR_MAX (__IPVS_SVC_ATTR_MAX - 1)

/*
 * Attributes used to describe a destination (real server)
 *
 * Used inside nested attribute IPVS_CMD_ATTR_DEST
 */
enum {
	IPVS_DEST_ATTR_UNSPEC = 0,
	IPVS_DEST_ATTR_ADDR,		/* real server address */
	IPVS_DEST_ATTR_PORT,		/* real server port */

	IPVS_DEST_ATTR_FWD_METHOD,	/* forwarding method */
	IPVS_DEST_ATTR_WEIGHT,		/* destination weight */

	IPVS_DEST_ATTR_U_THRESH,	/* upper threshold */
	IPVS_DEST_ATTR_L_THRESH,	/* lower threshold */

	IPVS_DEST_ATTR_ACTIVE_CONNS,	/* active connections */
	IPVS_DEST_ATTR_INACT_CONNS,	/* inactive connections */
	IPVS_DEST_ATTR_PERSIST_CONNS,	/* persistent connections */

	IPVS_DEST_ATTR_STATS,		/* nested attribute for dest stats */
	__IPVS_DEST_ATTR_MAX,
};

#define IPVS_DEST_ATTR_MAX (__IPVS_DEST_ATTR_MAX - 1)

/*
 * Attributes describing a sync daemon
 *
 * Used inside nested attribute IPVS_CMD_ATTR_DAEMON
 */
enum {
	IPVS_DAEMON_ATTR_UNSPEC = 0,
	IPVS_DAEMON_ATTR_STATE,		/* sync daemon state (master/backup) */
	IPVS_DAEMON_ATTR_MCAST_IFN,	/* multicast interface name */
	IPVS_DAEMON_ATTR_SYNC_ID,	/* SyncID we belong to */
	__IPVS_DAEMON_ATTR_MAX,
};

#define IPVS_DAEMON_ATTR_MAX (__IPVS_DAEMON_ATTR_MAX - 1)

/*
 * Attributes used to describe service or destination entry statistics
 *
 * Used inside nested attributes IPVS_SVC_ATTR_STATS and IPVS_DEST_ATTR_STATS
 */
enum {
	IPVS_STATS_ATTR_UNSPEC = 0,
	IPVS_STATS_ATTR_CONNS,		/* connections scheduled */
	IPVS_STATS_ATTR_INPKTS,		/* incoming packets */
	IPVS_STATS_ATTR_OUTPKTS,	/* outgoing packets */
	IPVS_STATS_ATTR_INBYTES,	/* incoming bytes */
	IPVS_STATS_ATTR_OUTBYTES,	/* outgoing bytes */

	IPVS_STATS_ATTR_CPS,		/* current connection rate */
	IPVS_STATS_ATTR_INPPS,		/* current in packet rate */
	IPVS_STATS_ATTR_OUTPPS,		/* current out packet rate */
	IPVS_STATS_ATTR_INBPS,		/* current in byte rate */
	IPVS_STATS_ATTR_OUTBPS,		/* current out byte rate */
	__IPVS_STATS_ATTR_MAX,
};

#define IPVS_STATS_ATTR_MAX (__IPVS_STATS_ATTR_MAX - 1)

/* Attributes used in response to IPVS_CMD_GET_INFO command */
enum {
	IPVS_INFO_ATTR_UNSPEC = 0,
	IPVS_INFO_ATTR_VERSION,		/* IPVS version number */
	IPVS_INFO_ATTR_CONN_TAB_SIZE,	/* size of connection hash table */
	__IPVS_INFO_ATTR_MAX,
};

#define IPVS_INFO_ATTR_MAX (__IPVS_INFO_ATTR_MAX - 1)

#ifdef LIBIPVS_USE_NL
extern struct nla_policy ipvs_cmd_policy[IPVS_CMD_ATTR_MAX + 1];
extern struct nla_policy ipvs_service_policy[IPVS_SVC_ATTR_MAX + 1];
extern struct nla_policy ipvs_dest_policy[IPVS_DEST_ATTR_MAX + 1];
extern struct nla_policy ipvs_stats_policy[IPVS_STATS_ATTR_MAX + 1];
extern struct nla_policy ipvs_info_policy[IPVS_INFO_ATTR_MAX + 1];
extern struct nla_policy ipvs_daemon_policy[IPVS_DAEMON_ATTR_MAX + 1];
#endif

/* End of Generic Netlink interface definitions */

#endif	/* _IP_VS_H */
