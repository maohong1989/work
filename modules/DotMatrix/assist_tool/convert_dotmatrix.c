#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct lib
{
	int size;
	int width;
	int height;
	int type;
};

int main(int argv, char **argc)
{
	FILE *fd;
	char path[100] = {0};
	char conver_path[100] = "convert-";
	char a[16];
	long size;
	int  ret;
	struct lib *tmp = (struct lib *)a;
	if (argv < 2)
	{
		printf("please input the fontlib path:");	
		scanf("%s", path);
	}
	memcpy(path, argc[1], strlen(argc[1]));

	printf("the size:");
	scanf("%d", &tmp->size);
	printf("the width:");
	scanf("%d", &tmp->width);
	printf("the height:");
	scanf("%d", &tmp->height);
	printf("the type[0-ascii,1-chinese]:");
	scanf("%d", &tmp->type);

	strcat(conver_path, path);
	printf("the path is:%s\n", path);
	printf("the convert_convert_path is:%s\n", conver_path);
	printf("tmp->size   = [%d]\n", tmp->size);
	printf("tmp->width  = [%d]\n", tmp->width);
	printf("tmp->height = [%d]\n", tmp->height);
	printf("tmp->type   = [%d]\n", tmp->type);


	fd = fopen(path, "rb");
	if (NULL == fd)
	{
		printf("open erorr!\n");	
		return -1;
	}
	ret = fseek(fd, 0, SEEK_END);
	printf("ret is:[%d]\n", ret);
	size = ftell(fd);
	printf("%lx\n", size);

	ret = fseek(fd, 0, SEEK_SET);
	char *p;
	p = malloc(size);
	fread(p, size, 1, fd);

	FILE *fd1;
	fd1 = fopen(conver_path, "wb");
	if (NULL == fd1)
	{
		printf("open erorr!\n");	
		return -1;
	}
	fwrite(a, 16, 1, fd1);
	fwrite(p, size, 1, fd1);
	fclose(fd);
	fclose(fd1);
	return 0;
}
