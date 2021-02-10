#ifndef __FINDER_H__
#define __FINDER_H__

int checkExtension(const char *filename, const char *extension);
void dirRead(int queue, char *dirName);
void *finder(void *info);
#endif // __FINDER_H__