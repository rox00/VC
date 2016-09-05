using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;

namespace UYouMain
{
    [DataContract]
    public class Emv_Oper_OpenApp_Data
    {
        [DataMember]
        public String packageName= null;

        [DataMember]
        public String activityName=null;
    }

    [DataContract]
    public class Emv_Oper_Upgrade_Data
    {
        [DataMember(Name="pc")]
        public Emv_Oper_Upgrade_Data_Item pc=null;
        [DataMember(Name="box")]
        public Emv_Oper_Upgrade_Data_Item box=null;
    }

    [DataContract]
    public class Emv_Oper_Upgrade_Data_Item
    {
        [DataMember]
        public String vername=null;     //新版本名称
        [DataMember]
        public String vervalue=null;    //新版本版本号
        [DataMember]
        public String type=null;        //升级类型；0：全量升级，1：增量升级
        [DataMember]
        public String description =null; //本版本的描述
        [DataMember]
        public String downloadurl =null; //文件下载的URL地址
        [DataMember]
        public String mand =null;        //升级策略；0：非强制升级,1：强制升级
        [DataMember]
        public String md5 = null;       //文件的MD5校验值

    }
}
