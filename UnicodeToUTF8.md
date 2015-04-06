유니코드에서 UTF8로 전환하기

http://115.68.20.142/jh_private/?document_srl=337
원문 : http://sonbosun.tistory.com/7


//UNICODE 로 저장하기

> TCHAR szPath[.md](.md) = _T("1234567890");
> int nLen =_tcslen(szPath);
> CFile file(_T("D:\\Down\\test.xml"), CFile::modeCreate|CFile::modeWrite);
> TCHAR szTmp[2](2.md);
> WORD wd = 0xfeff;
> memcpy(szTmp, &wd, 2);
> file.Write(szTmp, 2);
> file.Write(szPath, nLen\*sizeof(TCHAR));
> file.Close();_

여기서 핵심은 FEFF 이다.
유니코드의 식별자란 것인데, 파일의 처음부분에 넣어주면 유니코드라고 선언하는 셈이 된다.
(원래 규격은 FFFE 일것이지만, IBM PC 는 리틀엔디언 방식이므로 바이트 위치를 반대로 두어야 한다.)




//UTF-8 로 저장하기
int UnicodeToUtf8(TCHAR**pUnicode, char****pUtf8)
{
> int len = ::WideCharToMultiByte(CP\_UTF8, 0, (LPCWSTR)pUnicode, -1, NULL, 0, NULL, NULL);
  * Utf8 = new char[len](len.md);
> ::WideCharToMultiByte(CP\_UTF8, 0, (LPCWSTR)pUnicode, -1,**pUtf8, len, NULL, NULL);

> return len;
}
> // UTF 0xEFBBBF
> TCHAR szTmp[2](2.md);
> WORD wd = 0xbbef;
> memcpy(szTmp, &wd, 2);
> file.Write(szTmp, 2);
> BYTE byte = 0xbf;
> memcpy(szTmp, &byte, 1);
> file.Write(szTmp, 1);
> char **pUtf8 = NULL;
> nLen = UnicodeToUtf8(szXML, &pUtf8);
> file.Write(pUtf8, nLen-1);
> delete [.md](.md)pUtf8;
UTF-8 도 마찬가지로 헤더를 조작하는데, 이번엔 3바이트짜리 0xEFBBBF 헤더를 파일 앞에 써 놓았다.**





이상은 노트패드로 각 종류별로 저장해가며 생노가다한 산물이다 (눈물난다 ㅠㅠ)


추가: 이해를 돕고자 각 타입별 저장 형태를 아스키 형태로 표현해 보았다.

메모장에 "abcd" 라고 입력하고 각각의 타입으로 저장후 헥사에디터로 열어보았다.
(코드값 사이의 스페이스는 무시해서 보면 된다. 가령, "FF FF" 는 2바이트일뿐이다.)

참고로 'a' 의 아스키값은 61.

ASCII: (순수한 값만 존재한다.)
61 62 63 64

UNICODE(Little Endian): (헤더가 존재하고 한바이트 알파벳이 두바이트로 표현되었다. 가령 'a' 는 61 00)
FF FE 61 00 62 00 63 00 64 00

UNICODE(Big Endian): (리틀엔디안과 다른점은 두바이트씩의 짝이 앞뒤로 바뀐점뿐. 가령 'a' 는 00 61. 헤더도 앞뒤가 바뀐점에 유의.)
FE FF 00 61 00 62 00 63 00 64

UTF-8 : (헤더가 3바이트란것에 유의. 알파벳은 1바이트로 표현되었다.)
EF BB BF 61 62 63 64



이번엔  메모장에 "한글" 라고 입력하고 각각의 타입으로 저장후 헥사에디터로 열어보았다.

참고로 '한글' 의 아스키값은 C7 D1 B1 DB.

ASCII: (순수한 값만 존재한다.)
C7 D1 B1 DB

UNICODE(Little Endian): (헤더는 알파벳과 같지만 내용부분의 코드가 확 바껴 버렸다! 당연한 결과인가.)
FF FE 5C D5 00 AE

UNICODE(Big Endian): (마찬가지로 한쌍씩의 바이트 위치가 바꼈다.)
FE FF D5 5C AE 00

UTF-8 : (헤더가 3바이트란것은 똑같지만 이게 어떻게 된일? 한글자에 3바이트가 사용되었다! 흐음... 이것은 좀 놀라운 일이다.)
EF BB BF ED 95 9C EA B8 80