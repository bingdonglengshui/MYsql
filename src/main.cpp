#include"DataBase.h"
int main()
{

	DbSetting setting;
	setting.host="tcp://127.0.0.1:3306/test";
	setting.password="1234";
	setting.username="root";
	setting.connpoolsize=5;
	setting.reconnmaxsize=2;
	shared_ptr<DataBase> ptr=DataBase::GetInstance();
	if(ptr!=nullptr)
	{
		ResultSet* ret=ptr->executeQuery("select * from users");
		while((ret->next())!=false)
		{
			cout<<ret->getUInt("uid")<<endl;
		}
	}

	return 0;

}
