// By Jacques
#include "XmlParser.h"
#include "Tools.h"
#ifdef WIN32
int CXmlParserDocument::TransmitTCP(SOCKET sock)
{
    string szPack;
    int sizePack = 1024;
    char *pBuffer = ConvertToChar();
    string szToSend = pBuffer;
    delete[]pBuffer;
    //	send(sock, szToSend.data(), szToSend.length(), 0);

    auto nPack = szToSend.length() / (sizePack);
    for (auto iPack = 0; iPack < nPack; iPack++)
    {
        szPack = szToSend.substr((int)(iPack*sizePack), sizePack);
        send(sock, szPack.data(), (int) szPack.length(), 0);
    }
    if (szToSend.length() % sizePack)
    {
        szPack = szToSend.substr(nPack*sizePack);
        send(sock, szPack.data(), (int) szPack.length(), 0);
    }

    return 0;
}
int CXmlParserDocument::ReceiveTCP(SOCKET sock)
{
    clear();
    char buffer[1025];
    Text.clear();
    for (;;)
    {
        int iReceive = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (iReceive > 0)
        {
            printf("Bytes received: %d\n", iReceive);

            buffer[iReceive] = 0;
            wchar_t*pWbuffer = CTools::charToWchar(buffer);

            Text += pWbuffer;
            delete[] pWbuffer;
            if (CheckIfCompleted() == 0)
            {
                break;
            }

        }
    }
    Parse();
    return 0;
}

#endif