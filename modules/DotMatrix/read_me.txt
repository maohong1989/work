1������
	��ģ����Ҫ���ܣ����ݵ����ֿ�ȡ��λͼ

2����ע��
	1�� ��ʹ�ø������ߣ�
		a:�������ֿ���������     �������ɵ����ֿ�
		b:��convert_dotmatrix.c�����Խ������ֿ����ͷ������ͷΪ
		{
			int size;
			int width;
			int height;
			int type��0-ascii,1-chinese��;
		}��ǰ16���ַ�������������С����ȣ��߶ȣ����͡�
	
	2�� ���粻��ʹ��demo,�������롣���������г���
		locale -a  �鿴��ǰ֧�ֱ��룬����û��zh_CN.gbk����
		sudo vi /var/lib/locales/supported.d/local
		���
		zh_CN.GBK GBK
	