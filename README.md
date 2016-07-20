# VC
工程配置
VC09
..\..\Bin\VC09\$(ProjectName)\$(Configuration)


..\..\Intermediate\VC09\$(ProjectName)\$(Configuration)


VC12
			输出目录
..\..\Bin\VC12\$(ProjectName)\$(Configuration)
	中继目录
..\..\Intermediate\VC12\$(ProjectName)\$(Configuration)
创建目录时取消息 创建解决方案目录的选项


xcopy "$(TargetDir)$(TargetFileName)" "$(TargetDir)..\..\sdk\$(Configuration)\" /Y