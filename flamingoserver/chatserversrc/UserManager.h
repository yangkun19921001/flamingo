/** 
 *  �������е��û���Ϣ����ʼ��Ϣ�����ݿ��м���, UserManager.h
 *  zhangyl 2017.03.15
 **/

#pragma once
#include <stdint.h>
#include <string>
#include <list>
#include <mutex>
#include <set>

using namespace std;

#define GROUPID_BOUBDARY   0x0FFFFFFF 

enum FRIEND_OPERATION
{
    FRIEND_OPERATION_ADD,
    FRIEND_OPERATION_DELETE
};

//�û�����Ⱥ
struct User
{
    int32_t        userid;      //0x0FFFFFFF������Ⱥ�ţ���������ͨ�û�
    string         username;    //Ⱥ�˻���usernameҲ��Ⱥ��userid���ַ�����ʽ
    string         password;
    string         nickname;    //Ⱥ�˺�ΪȺ����
    int32_t        facetype;
    string         customface;
    string         customfacefmt;//�Զ���ͷ���ʽ
    int32_t        gender;
    int32_t        birthday;
    string         signature;
    string         address;
    string         phonenumber;
    string         mail;
    /*
    �����û����ѷ�����Ϣ������Ⱥ�˻���Ϊ�գ�����:
    [
    {
        "teamindex": 0,
        "teamname": "�ҵĺ���",
        "members": [
            {
                "userid": 1,
                "markname": "��ĳĳ"
            },
            {
                "userid": 2,
                "markname": "��xx"
            }
        ]
    },
    {
        "teamindex": 1,
        "teamname": "�ҵ�����",
        "members": [
            {
                "userid": 3,
                "markname": "��ĳĳ"
            },
            {
                "userid": 4,
                "markname": "��xx"
            }
        ]
    }
]
    */
    string         teaminfo;       //������ͨ�û���Ϊ������Ϣ������Ⱥ����Ϊ��
    int32_t        ownerid;        //����Ⱥ�˺ţ�ΪȺ��userid
    set<int32_t>   friends;        //Ϊ�˱����ظ�
};

class UserManager final
{
public:
    UserManager();
    ~UserManager();

    bool Init(const char* dbServer, const char* dbUserName, const char* dbPassword, const char* dbName);

    UserManager(const UserManager& rhs) = delete;
    UserManager& operator=(const UserManager& rhs) = delete;

    bool AddUser(User& u);
    bool MakeFriendRelationship(int32_t smallUserid, int32_t greaterUserid);
    bool ReleaseFriendRelationship(int32_t smallUserid, int32_t greaterUserid);
    bool AddFriendToUser(int32_t userid, int32_t friendid);
    bool DeleteFriendToUser(int32_t userid, int32_t friendid);

    //TODO: ���඼�Ǹ����û���Ϣ�Ľӿڣ��������Կ���ͳһ����
    bool UpdateUserInfoInDb(int32_t userid, const User& newuserinfo);
    bool ModifyUserPassword(int32_t userid, const std::string& newpassword);
    bool UpdateUserTeamInfo(int32_t userid, int32_t target, FRIEND_OPERATION operation);
    bool UpdateUserTeamInfoInDb(int32_t userid, const std::string& newteaminfo);

    bool AddGroup(const char* groupname, int32_t ownerid, int32_t& groupid);

#ifdef FXN_VERSION
    bool InsertDeviceInfo(int32_t userid, int32_t deviceid, int32_t classtype, int64_t uploadtime, const std::string& deviceinfo);
#endif

    //������Ϣ���
    bool SaveChatMsgToDb(int32_t senderid, int32_t targetid, const std::string& chatmsg);

    //TODO: ���û�Խ��Խ�࣬������Խ��Խ���ʱ�����ϵ�еĺ���Ч�ʸ���
    bool GetUserInfoByUsername(const std::string& username, User& u);
    bool GetUserInfoByUserId(int32_t userid, User& u);
    bool GetFriendInfoByUserId(int32_t userid, std::list<User>& friends);
    bool GetTeamInfoByUserId(int32_t userid, std::string& teaminfo);

private:
    bool LoadUsersFromDb();
    bool LoadRelationshipFromDb(int32_t userid, std::set<int32_t>& r);
    bool MakeUpTeamInfo(User& u, const set<int32_t>& friends);

private:
    int                 m_baseUserId{ 0 };        //m_baseUserId, ȡ���ݿ�����userid���ֵ�������û�����������ϵ���
    int                 m_baseGroupId{0x0FFFFFFF};
    list<User>          m_allCachedUsers;
    mutex               m_mutex;

    string              m_strDbServer;
    string              m_strDbUserName;
    string              m_strDbPassword;
    string              m_strDbName;
};