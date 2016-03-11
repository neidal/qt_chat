#ifndef __FCOMMONFUNC_H_
#define __FCOMMONFUNC_H_

// ³£ÓÃ×Ö·û´úÂëÒ³
#define CP_CHS		(936)
#define CP_CHT		(950)


class CommonFunc
{
public:
	CommonFunc();
	virtual ~CommonFunc();
	static std::string GenFileMD5(std::string sFile);
};

#endif
