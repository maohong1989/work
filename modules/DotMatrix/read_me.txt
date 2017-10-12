1、功能
	此模块主要功能：根据点阵字库取出位图

2、备注：
	1： 需使用辅助工具：
		a:【点阵字库生成器】     可以生成点阵字库
		b:【convert_dotmatrix.c】可以将点阵字库加上头，其中头为
		{
			int size;
			int width;
			int height;
			int type【0-ascii,1-chinese】;
		}即前16个字符里包换了字体大小，宽度，高度，类型。
	
	2： 假如不能使用demo,出现乱码。请运行下列程序。
		locale -a  查看当前支持编码，假如没有zh_CN.gbk，则
		sudo vi /var/lib/locales/supported.d/local
		添加
		zh_CN.GBK GBK
	