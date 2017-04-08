# VC
工程配置
VC12
	输出目录
$(ProjectDir)..\..\output\$(ProjectName)\$(Configuration)\
	中继目录
$(ProjectDir)..\..\Intermediate\$(ProjectName)\$(Configuration)\

	PDB输出 链接器 =》调试里面
$(ProjectDir)..\..\output\pdb\$(TargetName).pdb



包含目录
..\..\include\ffmpeg-2.4.5-win32-dev\include

库目录
..\..\include\ffmpeg-2.4.5-win32-dev\lib


创建目录时取消息 创建解决方案目录的选项（不必）

xcopy "$(TargetDir)$(TargetFileName)" "$(TargetDir)..\..\sdk\$(Configuration)\" /Y



WPF 在 MoseIntelligent.csproj 文件中设置

	中继目录
	<BaseIntermediateOutputPath>$(ProjectDir)..\..\Intermediate\$(AssemblyName)\</BaseIntermediateOutputPath>
	输出目录 Debug  Release 都要设置
	<PdbFile>$(ProjectDir)..\..\output\pdb\$(AssemblyName).pdb</PdbFile>
	<OutputPath>$(ProjectDir)..\..\output\$(AssemblyName)\$(Configuration)\</OutputPath>