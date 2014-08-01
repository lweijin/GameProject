#ifndef _INI_FILE_H_
#define _INI_FILE_H_

class CIniFile
{
public:
	CIniFile(void);

	~CIniFile(void);

public:
	BOOL Load(std::string strFileName);

	std::string GetStringValue(std::string strName)
	{
		std::map<std::string,std::string>::iterator itor = m_Values.find(strName);
		if(itor != m_Values.end())
		{
			return itor->second;
		}

		return "";
	}

	INT32 GetIntValue( std::string VarName)
	{
		return atoi(GetStringValue(VarName).c_str());
	}

	FLOAT GetFloatValue( std::string VarName)
	{
		return (float)atof(GetStringValue(VarName).c_str());
	}

	DOUBLE GetDoubleValue( std::string VarName)
	{
		return atof(GetStringValue(VarName).c_str());
	}




private:
	std::map<std::string,std::string> m_Values;

};

#endif