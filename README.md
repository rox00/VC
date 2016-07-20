# VC
工程配置
VC12
	输出目录
$(ProjectDir)..\..\output\$(ProjectName)\$(Configuration)\
	中继目录
$(ProjectDir)..\..\Intermediate\$(ProjectName)\$(Configuration)\

包含目录
..\..\include\ffmpeg-2.4.5-win32-dev\include

库目录
..\..\include\ffmpeg-2.4.5-win32-dev\lib


创建目录时取消息 创建解决方案目录的选项（不必）

xcopy "$(TargetDir)$(TargetFileName)" "$(TargetDir)..\..\sdk\$(Configuration)\" /Y