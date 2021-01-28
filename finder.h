#ifndef __FINDER_H__
#define __FINDER_H__

int checkExtension(const char *filename, const char *extension);
void dirRead(int queue, char *dirName, const char arr[32][1024], int extLen);
void *finder(void *info);
#endif // __FINDER_H__