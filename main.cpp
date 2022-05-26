#include <iostream>
#include <fstream>
#include <string.h>
#include <bitset>

using namespace std;

int main()
{
    fstream file ("Paquetes Redes/ethernet_ipv4_udp_dns.bin", ios::in | ios::binary);
    int cabeceraEthernet[14];
    int i;
    char bits[9], byte2[17], byte3[33];

    cout << "\n----------------------------------------------------------------------------------------------------\n"
           << "------                                        ETHERNET                                       -------\n"
           << "----------------------------------------------------------------------------------------------------\n"<< endl;

    if(file.fail())
        cout << "Ha ocurrido un error al abrir el archivo" << endl;
    else
    {
        cout << "Datos: ";
        for(i = 0; i < 14; i++){
            cabeceraEthernet[i] = file.get();
            cout << hex << cabeceraEthernet[i] << " ";
        }

        cout << endl << endl << "Direccion MAC de origen: ";
        for(i = 0; i < 6; i++){
            cout << hex << cabeceraEthernet[i];
            if(i < 5) cout << ":";
        }

        cout << endl << "Direccion MAC de destino: ";
        for(; i < 12; i++){
            cout << hex << cabeceraEthernet[i];
            if(i < 11) cout << ":";
        }

        cout << endl << endl << "Tipo de codigo: ";
        if(cabeceraEthernet[12] == 8 && cabeceraEthernet[13] == 0){ //0800 en hexadecimal
            /**  IPv4  **/
            cout << "\n----------------------------------------------------------------------------------------------------\n"
                   << "------                                           IPv4                                        -------\n"
                   << "----------------------------------------------------------------------------------------------------\n"<< endl;
            char version[5] = " ", ihl[5];
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());

            /**  Version  **/
            strncpy(version, bits, 4);
            if(strcmp(version, "0110") == 0)
                cout << version << " = Version: 6" << endl;
            else if(strcmp(version, "0100") == 0)
                cout << version << " = Version: 4" << endl;
            else
                cout << "Version no encontrada" << endl;

            /**  Tamano de cabecera (IHL)  **/
            strncpy(ihl, &bits[4], 5);
            string tamanio = ihl;
            unsigned long palabras = bitset<4>(tamanio).to_ulong();
            cout << ihl << " = Tamano de cabecera (IHL): " << dec << palabras << " palabras, " << dec << palabras*32 << " bits" << endl;

            /**  Tipo de servicio  **/
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            cout << endl << bits << "\t\tTipo de servicio: ";
            if(strncmp(bits, "000", 3) == 0)
                cout << "De rutina" << endl;
            else if(strncmp(bits, "001", 3) == 0)
                cout << "Prioritario" << endl;
            else if(strncmp(bits, "010", 3) == 0)
                cout << "Inmediato" << endl;
            else if(strncmp(bits, "011", 3) == 0)
                cout << "Relï¿½mpago" << endl;
            else if(strncmp(bits, "100", 3) == 0)
                cout << "Invalidacion relampago" << endl;
            else if(strncmp(bits, "101", 3) == 0)
                cout << "Procesando llamada critica y de emergencia" << endl;
            else if(strncmp(bits, "110", 3) == 0)
                cout << "Control de trabajo de internet" << endl;
            else if(strncmp(bits, "111", 3) == 0)
                cout << "Control de red" << endl;

            if(bits[3] == '0')
                cout << "\t\t\tRetardo normal" << endl;
            else
                cout << "\t\t\tRetardo bajo" << endl;
            if(bits[4] == '0')
                cout << "\t\t\tRendimiento normal" << endl;
            else
                cout << "\t\t\tRendimiento alto" << endl;
            if(bits[5] == '0')
                cout << "\t\t\tFiabilidad normal" << endl;
            else
                cout << "\t\t\tFiabilidad alta" << endl;

            /**  Longitud total  **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2 ;
            unsigned long tamano_total = bitset<16>(byte2).to_ulong();
            cout << "\tLongitud total: " << dec << tamano_total << endl;

            /** Identificador **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2 ;
            unsigned long identificador = bitset<16>(byte2).to_ulong();
            cout << "\tIdentificador: " << dec << identificador << endl;

            /**  Flags (banderas)  **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            char flag[4] = " ";
            strncpy(flag, byte2, 3);
            cout << endl << flag;
            cout << "\t\t\tFlags: " << endl;
            if(flag[1] == '0')
                cout << "\t\t\t\tPaquete divisible" << endl;
            else
                cout << "\t\t\t\tPaquete no divisible" << endl;
            if(flag[2] == '0')
                cout << "\t\t\t\tUltimo fragmento" << endl;
            else
                cout << "\t\t\t\tFragmento intermedio" << endl;

            /**  Posicion de fragmentos  **/
            for(int j = 0; j < 13; j++)
                byte2[j] = byte2[j+3];
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2;
            unsigned long posicion = bitset<16>(byte2).to_ulong();
            cout << "\t\tPosicion de fragmento: " << dec << posicion << endl;


            /**  Tiempo de Vida (TTL)  **/
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            cout << endl << bits;
            unsigned long ttl = bitset<16>(bits).to_ulong();
            cout << "\t\tTTL: " << dec << ttl << endl;

            /**  Protocolo  **/
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            cout << endl << bits;
            unsigned long protocolo = bitset<16>(bits).to_ulong();
            cout << "\t\tProtocolo: " << dec << protocolo << endl;
            if(protocolo == 1)
                cout<<"\t\t\tICMPv4"<<endl;
            else if(protocolo == 6)
                cout<<"\t\t\tTCP"<<endl;
            else if(protocolo == 17)
                cout<<"\t\t\tUDP"<<endl;
            else if(protocolo == 58)
                cout<<"\t\t\tICMPv6"<<endl;
            else if(protocolo == 118)
                cout<<"\t\t\tSTP"<<endl;
            else if(protocolo == 121)
                cout<<"\t\t\tSMP"<<endl;
            else
                cout<<"\t\t\tProtocolo no encontrado"<<endl;

            /**  Checksum  **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2;
            unsigned long checksum = bitset<16>(byte2).to_ulong();
            cout << "\tChecksum Hex: " << hex << checksum << endl;

            /**  Direccion IP-Origen  **/
            unsigned long ipOrigen;
            cout << endl << "\t\t\tDireccion IP-Origen: ";
            for(int i = 0; i < 4; i++){
                strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                ipOrigen = bitset<8>(bits).to_ulong();
                cout << dec << ipOrigen;
                if(i < 3) cout << ".";
            }

            /**  Direccion IP-Destino  **/
            unsigned long ipDestino;
            cout << endl << "\t\t\tDireccion IP-Destino: ";
            for(int i = 0; i < 4; i++){
                strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                ipDestino = bitset<8>(bits).to_ulong();
                cout << dec << ipDestino;
                if(i < 3) cout << ".";
            }

            /**   ---ICMPv4---   **/
            if(protocolo == 1){
                /**  Tipo  **/
                cout << endl << "\n\n------                                          ICMPv4                                       -------" << endl;
                strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                cout << endl << bits;
                unsigned long tipoICMP = bitset<8>(bits).to_ulong();
                cout <<  "\t\tTipo: " << "(" << dec << tipoICMP << ") ";
                if(tipoICMP == 0)
                    cout << "Eco reply \n\t\t\t\t  (Respuesta de eco)" << endl;
                else if(tipoICMP == 3)
                    cout << "Destination unreachable \n\t\t\t\t  (Destino inaccesible)" << endl;
                else if(tipoICMP == 4)
                    cout << "Source Quench \n\t\t\t\t  (Disminucion del trafico desde el origen)" << endl;
                else if(tipoICMP == 5)
                    cout << "Redirect \n\t\t\t\t  (Redireccionar - cambio de ruta)" << endl;
                else if(tipoICMP == 8)
                    cout << "Echo \n\t\t\t\t  (Solicitud de eco)" << endl;
                else if(tipoICMP == 11)
                    cout << "Time exceeded \n\t\t\t\t  (Tiempo excedido para un datagrama" << endl;
                else if(tipoICMP == 12)
                    cout << "Parameter problem \n\t\t\t\t  (Problema de parametros)" << endl;
                else if(tipoICMP == 13)
                    cout << "Time stamp \n\t\t\t\t  (Solicitud de marca de tiempo)" << endl;
                else if(tipoICMP == 14)
                    cout << "Time stamp reply \n\t\t\t\t  (Respuesta de marca de tiempo)" << endl;
                else if(tipoICMP == 15)
                    cout << "Information request \n\t\t\t\t  (Solicitud de informacion)" << endl;
                else if(tipoICMP == 16)
                    cout << "Information reply \n\t\t\t\t  (Respuesta de informacion)" << endl;
                else if(tipoICMP == 17)
                    cout << "Address mask \n\t\t\t\t  (Solicitud de mascara de direaccion)" << endl;
                else if(tipoICMP == 18)
                    cout << "Address mask reply \n\t\t\t\t  (Respuesta de mascara de direccion)" << endl;

                /**  Code  **/
                strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                cout << endl <<  bits;
                unsigned long code = bitset<8>(bits).to_ulong();
                cout << "\t\tCodigo de error: " << "\n\t\t\t(" << dec << code << ") ";
                if(code == 0)
                    cout << "No se puede llegar a la red" << endl;
                else if(code == 1)
                    cout << "No se puede llegar al host o aplicacion de destino" << endl;
                else if(code == 2)
                    cout << "El destino no dispone del protocolo solicitado" << endl;
                else if(code == 3)
                    cout << "No se puede llegar al puerto destino o la aplicacion destino no estÃ¡ libre" << endl;
                else if(code == 4)
                    cout << "Se necesita aplicar fragmentacion, pero el flag correspondiente indica lo contrario" << endl;
                else if(code == 5)
                    cout << "La ruta de origen no es correcta" << endl;
                else if(code == 6)
                    cout << "No se conoce la red destino" << endl;
                else if(code == 7)
                    cout << "No se conoce el host destino" << endl;
                else if(code == 8)
                    cout << "El host origen esta aislado" << endl;
                else if(code == 9)
                    cout << "La comunicaciÃ³n con la red destino estÃ¡ prohibida por razones administrativas" << endl;
                else if(code == 10)
                    cout << "La comunicaciÃ³n con el host destino estÃ¡ prohibida por razones administrativas" << endl;
                else if(code == 11)
                    cout << "No se puede llegar a la red destino debido al Tipo de servicio" << endl;
                else if(code == 12)
                    cout << "No se puede llegar al host destino debido al Tipo de servicio" << endl;

                /** Checksum  **/
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                checksum = bitset<16>(byte2).to_ulong();
                cout << "\tChecksum Hex: " << hex << checksum << endl;
            }
            /**   ---TCP---   **/
            if(protocolo == 6){
                /**  Tipo  **/
                cout << endl << "\n\n------                                            TCP                                        -------" << endl;
                /**  Puerto Origen  **/
                unsigned long pOrigen;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                pOrigen = bitset<16>(byte2).to_ulong();
                cout << "\tPuerto Origen: (" << dec << pOrigen << ") ";
                if(pOrigen >= 0 && pOrigen <= 1023){
                    cout << " Puertos bien conocidos" << endl << "\t\t\t";
                    if(pOrigen == 20){
                        cout << "Puerto: 20" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 21){
                        cout << "Puerto: 21" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 22){
                        cout << "Puerto: 22" << endl << "\t\t\tServicio: SSH" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 23){
                        cout << "Puerto: 23" << endl << "\t\t\tServicio: TELNET" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 25){
                        cout << "Puerto: 25" << endl << "\t\t\tServicio: SMTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 53){
                        cout << "Puerto: 53" << endl << "\t\t\tServicio: DNS" << endl << "\t\t\tProtocolo: TCP/UDP" << endl;
                    }
                    else if(pOrigen == 67){
                        cout << "Puerto: 67" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 68){
                        cout << "Puerto: 68" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 69){
                        cout << "Puerto: 69" << endl << "\t\t\tServicio: TFTP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 80){
                        cout << "Puerto: 80" << endl << "\t\t\tServicio: HTTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 110){
                        cout << "Puerto: 110" << endl << "\t\t\tServicio: POP3" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 143){
                        cout << "Puerto: 143" << endl << "\t\t\tServicio: IMAP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 443){
                        cout << "Puerto: 443" << endl << "\t\t\tServicio: HTTPS" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 993){
                        cout << "Puerto: 993" << endl << "\t\t\tServicio: IMAP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 995){
                        cout << "Puerto: 995" << endl << "\t\t\tServicio: POP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                }
                else if(pOrigen >= 1024 && pOrigen <= 49151)
                    cout << " Puertos registrados" << endl;
                else if(pOrigen >= 49151 && pOrigen <= 65535)
                    cout << " Puertos dinamicos o privados" << endl;

                /**  Puerto Destino  **/
                unsigned long pDestino;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                pDestino = bitset<16>(byte2).to_ulong();
                cout << "\tPuerto Destino: (" << dec << pDestino << ") ";
                if(pDestino >= 0 && pDestino <= 1023){
                    cout << " Puertos bien conocidos" << endl << "\t\t\t";
                    if(pDestino == 20){
                        cout << "Puerto: 20" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 21){
                        cout << "Puerto: 21" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 22){
                        cout << "Puerto: 22" << endl << "\t\t\tServicio: SSH" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 23){
                        cout << "Puerto: 23" << endl << "\t\t\tServicio: TELNET" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 25){
                        cout << "Puerto: 25" << endl << "\t\t\tServicio: SMTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 53){
                        cout << "Puerto: 53" << endl << "\t\t\tServicio: DNS" << endl << "\t\t\tProtocolo: TCP/UDP" << endl;
                    }
                    else if(pDestino == 67){
                        cout << "Puerto: 67" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 68){
                        cout << "Puerto: 68" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 69){
                        cout << "Puerto: 69" << endl << "\t\t\tServicio: TFTP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 80){
                        cout << "Puerto: 80" << endl << "\t\t\tServicio: HTTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 110){
                        cout << "Puerto: 110" << endl << "\t\t\tServicio: POP3" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 143){
                        cout << "Puerto: 143" << endl << "\t\t\tServicio: IMAP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 443){
                        cout << "Puerto: 443" << endl << "\t\t\tServicio: HTTPS" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 993){
                        cout << "Puerto: 993" << endl << "\t\t\tServicio: IMAP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 995){
                        cout << "Puerto: 995" << endl << "\t\t\tServicio: POP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                }
                else if(pDestino >= 1024 && pDestino <= 49151)
                    cout << " Puertos registrados" << endl;
                else if(pDestino >= 49151 && pDestino <= 65535)
                    cout << " Puertos dinamicos o privados" << endl;   

                /**  Número de secuencia  **/
                unsigned long num_secuencia;
                strcpy(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte3;
                num_secuencia = bitset<32>(byte3).to_ulong();
                cout << "\tNumero de secuencia: " << dec << num_secuencia;

                /**  Número de acuse de recibo  **/
                unsigned long num_acuse;
                strcpy(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte3;
                num_acuse = bitset<32>(byte3).to_ulong();
                cout << "\tNumero de acuse de recibo: " << dec << num_acuse << endl;

                /**  Longitud de Cabecera  **/
                strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                strncpy(ihl, bits, 4); 
                string tamano = ihl;
                unsigned long palabrastcp = bitset<8>(tamano).to_ulong();
                cout << endl << ihl << "\t\t\tLongitud de cabecera: " << dec << palabras << " palabras, " << dec << palabrastcp*32 << " bits" << endl;

                /**  Reservado  **/
                for(i = 0; i < 4; i++){
                    bits[i] = bits[i+4];
                }
              
                char reservadotcp[4] = " ";
                strncpy(reservadotcp, bits, 3);
                cout << endl << reservadotcp;
                cout << "\t\t\tReservado: " << reservadotcp;

                /**  Banderas  **/
                string b;
                b = bits[3];
                unsigned long band1 = bitset<1>(b).to_ulong();
                cout << endl << endl << "\t\t\tBanderas de comunicacion de TCP: " << endl;
                if(band1 == 1){
                    cout << "\t\t\t\tNS:  (" << band1 << ") ECN-nonce concealment protection." << endl;
                }
                strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                b = bits[0];
                unsigned long band2 = bitset<1>(b).to_ulong();
                if(band2 == 1){
                    cout << "\t\t\t\tCWR: (" << band2 << ") Congestion Window Reduced." << endl;
                }
                b = bits[1];
                unsigned long band3 = bitset<1>(b).to_ulong();
                if(band3 == 1){
                    cout << "\t\t\t\tECE: (" << band3 << ") Indicaciones sobre congestion." << endl;
                }
                b = bits[2];
                unsigned long band4 = bitset<1>(b).to_ulong();
                if(band4 == 1){
                    cout << "\t\t\t\tURG: (" << band4 << ") Campo de puntero urgente es valido." << endl;
                }
                b = bits[3];
                unsigned long band5 = bitset<1>(b).to_ulong();
                if(band5 == 1){
                    cout << "\t\t\t\tACK: (" << band5 << ") Acknowledge." << endl;
                }
                b = bits[4];
                unsigned long band6 = bitset<1>(b).to_ulong();
                if(band6 == 1){
                    cout << "\t\t\t\tPHS: (" << band6 << ") Push." << endl;
                }
                b = bits[5];
                unsigned long band7 = bitset<1>(b).to_ulong();
                if(band7 == 1){
                    cout << "\t\t\t\tRST: (" << band7 << ") Reset." << endl;
                }
                b = bits[6];
                unsigned long band8 = bitset<1>(b).to_ulong();
                if(band8 == 1){
                    cout << "\t\t\t\tSYN: (" << band8 << ") Synchronize." << endl;
                }
                b = bits[7];
                unsigned long band9 = bitset<1>(b).to_ulong();
                if(band9 == 1){
                    cout << "\t\t\t\tFIN: (" << band9 << ") Finish." << endl;
                }

                /**  Tamaño de ventana de recepción  **/
                unsigned long tamano_ventana;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                tamano_ventana = bitset<16>(byte2).to_ulong();
                cout << "\tTamano de ventana: " << dec << tamano_ventana;

                /**  Suma de verificación  **/
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                checksum = bitset<16>(byte2).to_ulong();
                cout << "\tChecksum: " << hex << checksum << endl;

                /**  Puntero urgente  **/
                unsigned long puntero_urgente;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                puntero_urgente = bitset<16>(byte2).to_ulong();
                cout << puntero_urgente << "\t\t\tPuntero urgente: " << dec << puntero_urgente;

                /**  Datos  **/
                int cabeceratcp[20];
                cout << endl << "\n\nDatos: ";
                for(i = 34; i < 53; i++){
                    cabeceratcp[i] = file.get();
                    cout << hex << cabeceratcp[i] << " ";
                }
                /**   ---DNS---   **/
                if(pDestino == 53){
                    /**  Tipo  **/
                    cout << endl << "\n\n------                                       Servicio DNS                                    -------" << endl;
                    /** ID **/
                    char OpCode[5] = " ", z[4] = " ", Rcode[5] = " ";
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << endl << byte2;
                    unsigned long ID = bitset<16>(byte2).to_ulong();
                    cout << "\tID: " << hex << ID << endl;

                    /** Banderas **/
                    cout << endl << "\t\t\tBanderas" << endl;
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << "\t\t\t\tQR: ";
                    if(byte2[0] == '0')
                        cout << "(0) Consulta" << endl;
                    else
                        cout << "(1) Respuesta" << endl;
                    strncpy(OpCode, &byte2[1], 4);
                    cout << OpCode;
                    cout<<"\t\t\t\tOp Code: ";
                    if(strcmp(OpCode, "0000") == 0)
                        cout << "(0) Consulta estandar(QUERY)" << endl;
                    else if(strcmp(OpCode, "0001") == 0)
                        cout << "(1) Consulta inversa(IQUERY)" << endl;
                    else if(strcmp(OpCode, "0010") == 0)
                        cout << "(2) solicitud del estado del servidor(STATUS)" << endl;
                    else
                        cout << "Reservados" <<endl;
                    if(byte2[5] == '0')
                        cout << "\t\t\t\tAA(0) No tiene respuesta autoritativa." << endl;
                    else
                        cout << "\t\t\t\tAA(1) Respuesta Autoritativa." << endl;
                    if(byte2[6] == '0')
                        cout << "\t\t\t\tTC(0) Respuesta no truncada." << endl;
                    else
                        cout << "\t\t\t\tTC(1) Respuesta truncada." << endl;
                    if(byte2[7] == '0')
                        cout << "\t\t\t\tRD(0) Recursion no deseada." << endl;
                    else
                        cout << "\t\t\t\tRD(1) Recursion deseada." << endl;
                    if(byte2[8] == '0')
                        cout << "\t\t\t\tRA(0) Recursividad no disponible." << endl;
                    else
                        cout << "\t\t\t\tRA(1) Recursividad Disponible." << endl;
                        
                    strncpy(z, &byte2[9], 3);
                    cout << z;
                    cout << "\t\t\t\tZ (Reservados)" << endl;
                    strncpy(Rcode, &byte2[12], 4);
                    cout << Rcode;
                    if(strcmp(Rcode, "0000") == 0)
                        cout << "\t\t\t\t(0) Ningun error." << endl;
                    else if(strcmp(Rcode, "0001") == 0)
                        cout << "\t\t\t\t(1) Error de formato." << endl;
                    else if(strcmp(Rcode, "0010") == 0)
                        cout << "\t\t\t\t(2) Fallo en el servidor." << endl;
                    else if(strcmp(Rcode, "0011") == 0)
                        cout << "\t\t\t\t(3) Error en nombre." << endl;
                    else if(strcmp(Rcode, "0100") == 0)
                        cout << "\t\t\t\t(4) No implementado." << endl;
                    else if(strcmp(Rcode, "0101") == 0)
                        cout << "\t\t\t\t(5) Rechazado." << endl;
                    else
                        cout << "Reservados." <<endl;

                    /** Contadores **/
                    cout << endl << "\t\t\tContadores" << endl;
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout  << byte2;
                    unsigned long QD = bitset<16>(byte2).to_ulong();
                    cout << "\t\tQDcount: (" << dec << QD << ") Preguntas" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long AN = bitset<16>(byte2).to_ulong();
                    cout << "\t\tANcount: (" << dec << AN << ") Respuestas" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long NS = bitset<16>(byte2).to_ulong();
                    cout << "\t\tNScount: (" << dec << NS << ") Autoridad" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long AR = bitset<16>(byte2).to_ulong();
                    cout << "\t\tARcount: (" << dec << AR << ") Registros adicionales" << endl << endl;

                    /**Campos de Pregunta**/
                    unsigned long puntero, numLetras, letra, tipoDNS, claseDNS;
                    char ascii;
                    cout << endl << "\t\t\tCampo de Pregunta" << endl;
                    cout << "Nombre del dominio: \t";
                    for(int i = 0; i < QD; i++){
                      numLetras = bitset<8>(file.get()).to_ulong();
                      do
                      {
                        cout << "<" << dec << numLetras << "><";
                        for(int i = 0; i < numLetras; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << ">";
                        numLetras = bitset<8>(file.get()).to_ulong();
                      }while(numLetras != 0);

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      tipoDNS = bitset<16>(byte2).to_ulong();
                      cout << endl << byte2;
                      cout << "\t\tTipo: ";
                      if(tipoDNS == 1)
                        cout << "A (1)"; 
                      else if(tipoDNS == 5)
                        cout << "CNAME (5)";
                      else if(tipoDNS == 13)
                        cout << "HINFO (13)";
                      else if(tipoDNS == 15)                           cout << "MX (15)";
                      else if(tipoDNS == 22 || tipoDNS == 23)
                        cout << "NS (22)";
                      else
                        cout << "No encontrado";

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      cout << "\t\tClase: ";
                      claseDNS = bitset<16>(byte2).to_ulong();
                      if(claseDNS == 1)
                        cout << "IN (1)" << endl; 
                      else if(claseDNS == 3)
                        cout << "CH (3)" << endl;
                      else
                        cout << "No encontrado" << endl;
                    }

                    /** Campos de respuesta **/
                    for(int i = 0; i < AN; i++){
                      cout << endl << "\t\t\tCampo de Respuesta" << endl;
                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      puntero = bitset<16>(byte2).to_ulong();
                      cout << "\t\tPTR: "<< hex << puntero << endl;

                      file.seekg(puntero);
                      numLetras = bitset<8>(file.get()).to_ulong();
                      cout << "Nombre del dominio: \t";
                      do
                      {
                        cout << "<" << dec << numLetras << "><";
                        for(int i = 0; i < numLetras; i++)
                        {
                          letra =  bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << ">";
                        numLetras = bitset<8>(file.get()).to_ulong();
                      } while(numLetras != 0);

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      tipoDNS = bitset<16>(byte2).to_ulong();
                      cout << endl << byte2;
                      cout << "\t\tTipo: ";
                      if(tipoDNS == 1)
                        cout << "A (1)"; 
                      else if(tipoDNS == 5)
                        cout << "CNAME (5)";
                      else if(tipoDNS == 13)
                        cout << "HINFO (13)";
                      else if(tipoDNS == 15)                           cout << "MX (15)";
                      else if(tipoDNS == 22 || tipoDNS == 23)
                        cout << "NS";
                      else
                        cout << "No encontrado";

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      cout << "\t\tClase: ";
                      claseDNS = bitset<16>(byte2).to_ulong();
                      if(claseDNS == 1)
                        cout << "IN (1)" << endl; 
                      else if(claseDNS == 3)
                        cout << "CH (3)" << endl;
                      else
                        cout << "No encontrado" << endl;

                      strcpy(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      unsigned long TTL = bitset<32>(byte3).to_ulong();
                      cout << "TTL: " << dec << TTL << endl;

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      unsigned long longDatos;
                      cout << "Logitud de datos: " << dec << longDatos << endl;

                      unsigned long Rdata;
                      cout << endl << "\n\t\t\tRDATA: ";
                      if(tipoDNS == 1){ // A
                        cout << endl << "\tDireccion IP: ";
                        for(int i = 0; i < 4; i++){
                          strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                          Rdata = bitset<8>(bits).to_ulong();
                          cout << dec << Rdata;
                          if(i < 3) cout << ".";
                        }
                      }
                      else if(tipoDNS == 5){ // CNAME
                        for(int i = 0; i < AN; i++){
                          numLetras = bitset<8>(file.get()).to_ulong();
                          cout << endl << "\tNombre del dominio: ";
                          do
                          {
                            for(int i = 0; i < numLetras; i++)
                            {
                              letra =  bitset<8>(file.get()).to_ulong();
                              ascii = letra;
                              cout << ascii;
                            }
                            cout << ".";
                            numLetras = bitset<8>(file.get()).to_ulong();
                          } while(numLetras != 0);
                        }
                      }
                      else if(tipoDNS == 15){ // MX
                        unsigned long prioridad;
                        strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                        strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                        cout << endl << byte2;
                        prioridad = bitset<16>(byte2).to_ulong();
                        cout << "\tPrioridad: " << prioridad;
                        cout << endl << "\tNombre del ordenador: ";
                        for(int i = 0; i < longDatos - 2; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << endl;
                      }
                      else if(tipoDNS == 22 || tipoDNS == 23){ // NS
                        cout << endl << "\tNombre del ordenador: ";
                        for(int i = 0; i < longDatos; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                      }
                    }
                }
            }
            /**   ---UDP---   **/
            if(protocolo == 17){
                /**  Tipo  **/
                cout << endl << "\n\n------                                            UDP                                        -------" << endl;
                /**  Puerto Origen  **/
                unsigned long pOrigen;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                pOrigen = bitset<16>(byte2).to_ulong();
                cout << "\tPuerto Origen: (" << dec << pOrigen << ") ";
                if(pOrigen >= 0 && pOrigen <= 1023){
                    cout << " Puertos bien conocidos" << endl << "\t\t\t";
                    if(pOrigen == 20){
                        cout << "Puerto: 20" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 21){
                        cout << "Puerto: 21" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 22){
                        cout << "Puerto: 22" << endl << "\t\t\tServicio: SSH" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 23){
                        cout << "Puerto: 23" << endl << "\t\t\tServicio: TELNET" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 25){
                        cout << "Puerto: 25" << endl << "\t\t\tServicio: SMTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 53){
                        cout << "Puerto: 53" << endl << "\t\t\tServicio: DNS" << endl << "\t\t\tProtocolo: TCP/UDP" << endl;
                    }
                    else if(pOrigen == 67){
                        cout << "Puerto: 67" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 68){
                        cout << "Puerto: 68" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 69){
                        cout << "Puerto: 69" << endl << "\t\t\tServicio: TFTP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 80){
                        cout << "Puerto: 80" << endl << "\t\t\tServicio: HTTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 110){
                        cout << "Puerto: 110" << endl << "\t\t\tServicio: POP3" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 143){
                        cout << "Puerto: 143" << endl << "\t\t\tServicio: IMAP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 443){
                        cout << "Puerto: 443" << endl << "\t\t\tServicio: HTTPS" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 993){
                        cout << "Puerto: 993" << endl << "\t\t\tServicio: IMAP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 995){
                        cout << "Puerto: 995" << endl << "\t\t\tServicio: POP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                }
                else if(pOrigen >= 1024 && pOrigen <= 49151)
                    cout << " Puertos registrados" << endl;
                else if(pOrigen >= 49151 && pOrigen <= 65535)
                    cout << " Puertos dinamicos o privados" << endl;

                /**  Puerto Destino  **/
                unsigned long pDestino;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                pDestino = bitset<16>(byte2).to_ulong();
                cout << "\tPuerto Destino: (" << dec << pDestino << ") ";
                if(pDestino >= 0 && pDestino <= 1023){
                    cout << " Puertos bien conocidos" << endl << "\t\t\t";
                    if(pDestino == 20){
                        cout << "Puerto: 20" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 21){
                        cout << "Puerto: 21" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 22){
                        cout << "Puerto: 22" << endl << "\t\t\tServicio: SSH" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 23){
                        cout << "Puerto: 23" << endl << "\t\t\tServicio: TELNET" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 25){
                        cout << "Puerto: 25" << endl << "\t\t\tServicio: SMTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 53){
                        cout << "Puerto: 53" << endl << "\t\t\tServicio: DNS" << endl << "\t\t\tProtocolo: TCP/UDP" << endl;
                    }
                    else if(pDestino == 67){
                        cout << "Puerto: 67" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 68){
                        cout << "Puerto: 68" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 69){
                        cout << "Puerto: 69" << endl << "\t\t\tServicio: TFTP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 80){
                        cout << "Puerto: 80" << endl << "\t\t\tServicio: HTTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 110){
                        cout << "Puerto: 110" << endl << "\t\t\tServicio: POP3" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 143){
                        cout << "Puerto: 143" << endl << "\t\t\tServicio: IMAP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 443){
                        cout << "Puerto: 443" << endl << "\t\t\tServicio: HTTPS" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 993){
                        cout << "Puerto: 993" << endl << "\t\t\tServicio: IMAP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 995){
                        cout << "Puerto: 995" << endl << "\t\t\tServicio: POP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                }
                else if(pDestino >= 1024 && pDestino <= 49151)
                    cout << " Puertos registrados" << endl;
                else if(pDestino >= 49151 && pDestino <= 65535)
                    cout << " Puertos dinamicos o privados" << endl;   

                /**  Longitud total  **/
                unsigned long longitud_total;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                longitud_total = bitset<16>(byte2).to_ulong();
                cout << "\tLongitud Total: " << dec << longitud_total;

                /**  Suma de verificación  **/
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                checksum = bitset<16>(byte2).to_ulong();
                cout << "\tChecksum: " << hex << checksum;

                /**  Datos  
                int cabeceraudp[8];
                cout << endl << "\n\nDatos: ";
                for(i = 34; i < 42; i++){
                    cabeceraudp[i] = file.get();
                    cout << hex << cabeceraudp[i] << " ";
                }**/

                /**   ---DNS---   **/
                if(pDestino == 53){
                    /**  Tipo  **/
                    cout << endl << "\n\n------                                       Servicio DNS                                    -------" << endl;
                    /** ID **/
                    char OpCode[5] = " ", z[4] = " ", Rcode[5] = " ";
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << endl << byte2;
                    unsigned long ID = bitset<16>(byte2).to_ulong();
                    cout << "\tID: " << hex << ID << endl;

                    /** Banderas **/
                    cout << endl << "\t\t\tBanderas" << endl;
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << "\t\t\t\tQR: ";
                    if(byte2[0] == '0')
                        cout << "(0) Consulta" << endl;
                    else
                        cout << "(1) Respuesta" << endl;
                    strncpy(OpCode, &byte2[1], 4);
                    cout << OpCode;
                    cout<<"\t\t\t\tOp Code: ";
                    if(strcmp(OpCode, "0000") == 0)
                        cout << "(0) Consulta estandar(QUERY)" << endl;
                    else if(strcmp(OpCode, "0001") == 0)
                        cout << "(1) Consulta inversa(IQUERY)" << endl;
                    else if(strcmp(OpCode, "0010") == 0)
                        cout << "(2) solicitud del estado del servidor(STATUS)" << endl;
                    else
                        cout << "Reservados" <<endl;
                    if(byte2[5] == '0')
                        cout << "\t\t\t\tAA(0) No tiene respuesta autoritativa." << endl;
                    else
                        cout << "\t\t\t\tAA(1) Respuesta Autoritativa." << endl;
                    if(byte2[6] == '0')
                        cout << "\t\t\t\tTC(0) Respuesta no truncada." << endl;
                    else
                        cout << "\t\t\t\tTC(1) Respuesta truncada." << endl;
                    if(byte2[7] == '0')
                        cout << "\t\t\t\tRD(0) Recursion no deseada." << endl;
                    else
                        cout << "\t\t\t\tRD(1) Recursion deseada." << endl;
                    if(byte2[8] == '0')
                        cout << "\t\t\t\tRA(0) Recursividad no disponible." << endl;
                    else
                        cout << "\t\t\t\tRA(1) Recursividad Disponible." << endl;
                        
                    strncpy(z, &byte2[9], 3);
                    cout << z;
                    cout << "\t\t\t\tZ (Reservados)" << endl;
                    strncpy(Rcode, &byte2[12], 4);
                    cout << Rcode;
                    if(strcmp(Rcode, "0000") == 0)
                        cout << "\t\t\t\t(0) Ningun error." << endl;
                    else if(strcmp(Rcode, "0001") == 0)
                        cout << "\t\t\t\t(1) Error de formato." << endl;
                    else if(strcmp(Rcode, "0010") == 0)
                        cout << "\t\t\t\t(2) Fallo en el servidor." << endl;
                    else if(strcmp(Rcode, "0011") == 0)
                        cout << "\t\t\t\t(3) Error en nombre." << endl;
                    else if(strcmp(Rcode, "0100") == 0)
                        cout << "\t\t\t\t(4) No implementado." << endl;
                    else if(strcmp(Rcode, "0101") == 0)
                        cout << "\t\t\t\t(5) Rechazado." << endl;
                    else
                        cout << "Reservados." <<endl;

                    /** Contadores **/
                    cout << endl << "\t\t\tContadores" << endl;
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout  << byte2;
                    unsigned long QD = bitset<16>(byte2).to_ulong();
                    cout << "\t\tQDcount: (" << dec << QD << ") Preguntas" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long AN = bitset<16>(byte2).to_ulong();
                    cout << "\t\tANcount: (" << dec << AN << ") Respuestas" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long NS = bitset<16>(byte2).to_ulong();
                    cout << "\t\tNScount: (" << dec << NS << ") Autoridad" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long AR = bitset<16>(byte2).to_ulong();
                    cout << "\t\tARcount: (" << dec << AR << ") Registros adicionales" << endl << endl;

                    /**Campos de Pregunta**/
                    unsigned long puntero, numLetras, letra, tipoDNS, claseDNS;
                    char ascii;
                    cout << endl << "\t\t\tCampo de Pregunta" << endl;
                    cout << "Nombre del dominio: \t";
                    for(int i = 0; i < QD; i++){
                      numLetras = bitset<8>(file.get()).to_ulong();
                      do
                      {
                        cout << "<" << dec << numLetras << "><";
                        for(int i = 0; i < numLetras; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << ">";
                        numLetras = bitset<8>(file.get()).to_ulong();
                      }while(numLetras != 0);

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      tipoDNS = bitset<16>(byte2).to_ulong();
                      cout << endl << byte2;
                      cout << "\t\tTipo: ";
                      if(tipoDNS == 1)
                        cout << "A (1)"; 
                      else if(tipoDNS == 5)
                        cout << "CNAME (5)";
                      else if(tipoDNS == 13)
                        cout << "HINFO (13)";
                      else if(tipoDNS == 15)                           cout << "MX (15)";
                      else if(tipoDNS == 22 || tipoDNS == 23)
                        cout << "NS (22)";
                      else
                        cout << "No encontrado";

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      cout << "\t\tClase: ";
                      claseDNS = bitset<16>(byte2).to_ulong();
                      if(claseDNS == 1)
                        cout << "IN (1)" << endl; 
                      else if(claseDNS == 3)
                        cout << "CH (3)" << endl;
                      else
                        cout << "No encontrado" << endl;
                    }

                    /** Campos de respuesta **/
                    for(int i = 0; i < AN; i++){
                      cout << endl << "\t\t\tCampo de Respuesta" << endl;
                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      puntero = bitset<16>(byte2).to_ulong();
                      cout << "\t\tPTR: "<< hex << puntero << endl;

                      file.seekg(puntero);
                      numLetras = bitset<8>(file.get()).to_ulong();
                      cout << "Nombre del dominio: \t";
                      do
                      {
                        cout << "<" << dec << numLetras << "><";
                        for(int i = 0; i < numLetras; i++)
                        {
                          letra =  bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << ">";
                        numLetras = bitset<8>(file.get()).to_ulong();
                      } while(numLetras != 0);

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      tipoDNS = bitset<16>(byte2).to_ulong();
                      cout << endl << byte2;
                      cout << "\t\tTipo: ";
                      if(tipoDNS == 1)
                        cout << "A (1)"; 
                      else if(tipoDNS == 5)
                        cout << "CNAME (5)";
                      else if(tipoDNS == 13)
                        cout << "HINFO (13)";
                      else if(tipoDNS == 15)                           cout << "MX (15)";
                      else if(tipoDNS == 22 || tipoDNS == 23)
                        cout << "NS";
                      else
                        cout << "No encontrado";

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      cout << "\t\tClase: ";
                      claseDNS = bitset<16>(byte2).to_ulong();
                      if(claseDNS == 1)
                        cout << "IN (1)" << endl; 
                      else if(claseDNS == 3)
                        cout << "CH (3)" << endl;
                      else
                        cout << "No encontrado" << endl;

                      strcpy(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      unsigned long TTL = bitset<32>(byte3).to_ulong();
                      cout << "TTL: " << dec << TTL << endl;

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      unsigned long longDatos;
                      cout << "Logitud de datos: " << dec << longDatos << endl;

                      unsigned long Rdata;
                      cout << endl << "\n\t\t\tRDATA: ";
                      if(tipoDNS == 1){ // A
                        cout << endl << "\tDireccion IP: ";
                        for(int i = 0; i < 4; i++){
                          strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                          Rdata = bitset<8>(bits).to_ulong();
                          cout << dec << Rdata;
                          if(i < 3) cout << ".";
                        }
                      }
                      else if(tipoDNS == 5){ // CNAME
                        for(int i = 0; i < AN; i++){
                          numLetras = bitset<8>(file.get()).to_ulong();
                          cout << endl << "\tNombre del dominio: ";
                          do
                          {
                            for(int i = 0; i < numLetras; i++)
                            {
                              letra =  bitset<8>(file.get()).to_ulong();
                              ascii = letra;
                              cout << ascii;
                            }
                            cout << ".";
                            numLetras = bitset<8>(file.get()).to_ulong();
                          } while(numLetras != 0);
                        }
                      }
                      else if(tipoDNS == 15){ // MX
                        unsigned long prioridad;
                        strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                        strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                        cout << endl << byte2;
                        prioridad = bitset<16>(byte2).to_ulong();
                        cout << "\tPrioridad: " << prioridad;
                        cout << endl << "\tNombre del ordenador: ";
                        for(int i = 0; i < longDatos - 2; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << endl;
                      }
                      else if(tipoDNS == 22 || tipoDNS == 23){ // NS
                        cout << endl << "\tNombre del ordenador: ";
                        for(int i = 0; i < longDatos; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                      }
                    }
                }
            }
        }
        else if (cabeceraEthernet[12] == 8 && cabeceraEthernet[13] == 6)
        { //0806 en hexadecimal
            /**  ARP  **/
            cout << "\n----------------------------------------------------------------------------------------------------\n"
                   << "------                                           ARP                                         -------\n"
                   << "----------------------------------------------------------------------------------------------------\n"<< endl;
            /*  Tipo de hardware  **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2;
            unsigned long tipo_hardware = bitset<16>(byte2).to_ulong();
            cout << "\tTipo de hardware: \t" << dec << tipo_hardware << ") ";
            if(tipo_hardware == 1)
                cout << "Ethernet (10 Mb)" << endl;
            else if(tipo_hardware == 6)
                cout << "IEEE 802 Networks" << endl;
            else if(tipo_hardware == 7)
                cout << "ARCNET" << endl;
            else if(tipo_hardware == 15)
                cout << "Frame Relay" << endl;
            else if(tipo_hardware == 16 || tipo_hardware == 19)
                cout << "Asynchronous Transfer Mode (ATM)" << endl;
            else if(tipo_hardware == 17)
                cout << "HDLC" << endl;
            else if(tipo_hardware == 18)
                cout << "Fibre Channel" << endl;
            else if(tipo_hardware == 20)
                cout << "Serial Line" << endl;
            else
                cout << "Tipo desconocido" << endl;

            /**  Tipo de protocolo  **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2;
            unsigned long tipo_protocolo = bitset<16>(byte2).to_ulong();
            cout << "\tTipo de protocolo: \t" << hex << tipo_protocolo << ") ";
            if(tipo_protocolo == 2048)
                cout << "IPv4" << endl;
            else if(tipo_protocolo == 2054)
                cout << "ARP" << endl;
            else if(tipo_protocolo == 32821)
                cout << "RARP" << endl;
            else if(tipo_protocolo == 34525)
                cout << "IPv6" << endl;
            else
                cout << "Ha ocurrido un error..." << endl;

            /**  Longitud de direccion hardware  **/
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            cout << endl << bits;
            unsigned long longitud_hardware = bitset<8>(bits).to_ulong();
            cout << "\t\tLongitud de la direccion de hardware: " << dec << longitud_hardware << endl;

            /**  Longitud de direccion protocolo  **/
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            cout << endl << bits;
            unsigned long longitud_protocolo = bitset<8>(bits).to_ulong();
            cout << "\t\tLongitud de la direccion de protocolo: " << dec << longitud_protocolo << endl;

            /** Codigo de operacion **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2;
            unsigned long codigo_operacion = bitset<16>(byte2).to_ulong();
            cout << "\tCodigo de operacion: \t" << dec << codigo_operacion << ") ";
            if(codigo_operacion == 1)
                cout << "Solicitud ARP" << endl;
            else if(codigo_operacion == 2)
                cout << "Respuesta ARP" << endl;
            else if(codigo_operacion == 3)
                cout << "Solicitud RARP" << endl;
            else if(codigo_operacion == 4)
                cout << "Respuesta ARP" << endl;

            /** Direccion hardware del emisor **/
            unsigned long hardw_Emisor;
            cout << endl << "\n\t\t\tDireccion hardware del emisor: ";
            for(int i = 0; i < 6; i++){
                strcpy(byte2, bitset<16>(file.get()).to_string().c_str());
                hardw_Emisor = bitset<16>(byte2).to_ulong();
                cout << hex << hardw_Emisor;
                if(i < 5) cout << ".";
            }

            /** Direccion IP del emisor **/
            unsigned long ipEmisor;
            cout << endl << "\n\t\t\tDireccion IP del emisor: ";
            for(int i = 0; i < 4; i++){
                strcpy(byte2, bitset<16>(file.get()).to_string().c_str());
                ipEmisor = bitset<16>(byte2).to_ulong();
                cout << dec << ipEmisor;
                if(i < 3) cout << ".";
            }

            /** Direccion hardware del receptor **/
            unsigned long hardw_Receptor;
            cout << endl << "\n\t\t\tDireccion hardware del receptor: ";
            for(int i = 0; i < 6; i++){
                strcpy(byte2, bitset<16>(file.get()).to_string().c_str());
                hardw_Receptor = bitset<16>(byte2).to_ulong();
                cout << hex << hardw_Receptor;
                if(i < 5) cout << ".";
            }

            /** Direccion IP del receptor **/
            unsigned long ipReceptor;
            cout << endl << "\n\t\t\tDireccion IP del receptor: ";
            for(int i = 0; i < 4; i++){
                strcpy(byte2, bitset<16>(file.get()).to_string().c_str());
                ipReceptor = bitset<16>(byte2).to_ulong();
                cout << dec << ipReceptor;
                if(i < 3) cout << ".";
            }
        }
        else if(cabeceraEthernet[12] == 128 && cabeceraEthernet[13] == 53){ //0806 en hexadecimal
            /**  RARP  **/
            cout << "\n----------------------------------------------------------------------------------------------------\n"
                   << "------                                           RARP                                         ------\n"
                   << "----------------------------------------------------------------------------------------------------\n"<< endl;

            /**  Tipo de hardware  **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2;
            unsigned long tipo_hardware = bitset<16>(byte2).to_ulong();
            cout << "\tTipo de hardware: \t" << dec << tipo_hardware << ") ";
            if(tipo_hardware == 1)
                cout << "Ethernet (10 Mb)" << endl;
            else if(tipo_hardware == 6)
                cout << "IEEE 802 Networks" << endl;
            else if(tipo_hardware == 7)
                cout << "ARCNET" << endl;
            else if(tipo_hardware == 15)
                cout << "Frame Relay" << endl;
            else if(tipo_hardware == 16 || tipo_hardware == 19)
                cout << "Asynchronous Transfer Mode (ATM)" << endl;
            else if(tipo_hardware == 17)
                cout << "HDLC" << endl;
            else if(tipo_hardware == 18)
                cout << "Fibre Channel" << endl;
            else if(tipo_hardware == 20)
                cout << "Serial Line" << endl;
            else
                cout << "Tipo desconocido" << endl;

            /**  Tipo de protocolo  **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2;
            unsigned long tipo_protocolo = bitset<16>(byte2).to_ulong();
            cout << "\tTipo de protocolo: \t" << hex << tipo_protocolo << ") ";
            if(tipo_protocolo == 2048)
                cout << "IPv4" << endl;
            else if(tipo_protocolo == 2054)
                cout << "ARP" << endl;
            else if(tipo_protocolo == 32821)
                cout << "RARP" << endl;
            else if(tipo_protocolo == 34525)
                cout << "IPv6" << endl;
            else
                cout << "Ha ocurrido un error..." << endl;

            /**  Longitud de direccion hardware  **/
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            cout << endl << bits;
            unsigned long longitud_hardware = bitset<8>(bits).to_ulong();
            cout << "\t\tLongitud de la direccion de hardware: " << dec << longitud_hardware << endl;

            /**  Longitud de direccion protocolo  **/
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            cout << endl << bits;
            unsigned long longitud_protocolo = bitset<8>(bits).to_ulong();
            cout << "\t\tLongitud de la direccion de protocolo: " << dec << longitud_protocolo << endl;

            /** Codigo de operacion **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2;
            unsigned long codigo_operacion = bitset<16>(byte2).to_ulong();
            cout << "\tCodigo de operacion: \t" << dec << codigo_operacion << ") ";
            if(codigo_operacion == 1)
                cout << "Solicitud ARP" << endl;
            else if(codigo_operacion == 2)
                cout << "Respuesta ARP" << endl;
            else if(codigo_operacion == 3)
                cout << "Solicitud RARP" << endl;
            else if(codigo_operacion == 4)
                cout << "Respuesta ARP" << endl;

            /** Direccion hardware del emisor **/
            unsigned long hardw_Emisor;
            cout << endl << "\n\t\t\tDireccion hardware del emisor: ";
            for(int i = 0; i < 6; i++){
                strcpy(byte2, bitset<16>(file.get()).to_string().c_str());
                hardw_Emisor = bitset<16>(byte2).to_ulong();
                cout << hex << hardw_Emisor;
                if(i < 5) cout << ".";
            }

            /** Direccion IP del emisor **/
            unsigned long ipEmisor;
            cout << endl << "\n\t\t\tDireccion IP del emisor: ";
            for(int i = 0; i < 4; i++){
                strcpy(byte2, bitset<16>(file.get()).to_string().c_str());
                ipEmisor = bitset<16>(byte2).to_ulong();
                cout << dec << ipEmisor;
                if(i < 3) cout << ".";
            }

            /** Direccion hardware del receptor **/
            unsigned long hardw_Receptor;
            cout << endl << "\n\t\t\tDireccion hardware del receptor: ";
            for(int i = 0; i < 6; i++){
                strcpy(byte2, bitset<16>(file.get()).to_string().c_str());
                hardw_Receptor = bitset<16>(byte2).to_ulong();
                cout << hex << hardw_Receptor;
                if(i < 5) cout << ".";
            }

            /** Direccion IP del receptor **/
            unsigned long ipReceptor;
            cout << endl << "\n\t\t\tDireccion IP del receptor: ";
            for(int i = 0; i < 4; i++){
                strcpy(byte2, bitset<16>(file.get()).to_string().c_str());
                ipReceptor = bitset<16>(byte2).to_ulong();
                cout << dec << ipReceptor;
                if(i < 3) cout << ".";
            }
        }
        else if(cabeceraEthernet[12] == 134 && cabeceraEthernet[13] == 221){ //86DD en hexadecimal
            /** "IPv6 **/
            cout << "\n----------------------------------------------------------------------------------------------------\n"
                   << "------                                           IPv6                                        -------\n"
                   << "----------------------------------------------------------------------------------------------------\n"<< endl;

            char version[5] = " ", traf[9], aux[5], byte3[21];
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());

            /**  Version  **/
            strncpy(version, bits, 4);
            if(strcmp(version, "0110") == 0)
                cout << version << " = Version: 6" << endl;
            else if(strcmp(version, "0100") == 0)
                cout << version << " = Version: 4" << endl;
            else
                cout << "Version no encontrada" << endl;

            /**  Clase de trÃ¡fico  **/
            strncpy(traf, &bits[4], 5);
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            strncpy(aux, bits, 4);
            strcat(traf, aux);
            cout << endl << traf << "\t\tClase de trafico: ";
            if(strncmp(traf, "000", 3) == 0)
                cout << "De rutina" << endl;
            else if(strncmp(traf, "001", 3) == 0)
                cout << "Prioritario" << endl;
            else if(strncmp(traf, "010", 3) == 0)
                cout << "Inmediato" << endl;
            else if(strncmp(traf, "011", 3) == 0)
                cout << "Relampago" << endl;
            else if(strncmp(traf, "100", 3) == 0)
                cout << "Invalidacion relampago" << endl;
            else if(strncmp(traf, "101", 3) == 0)
                cout << "Procesando llamada critica y de emergencia" << endl;
            else if(strncmp(traf, "110", 3) == 0)
                cout << "Control de trabajo de internet" << endl;
            else if(strncmp(traf, "111", 3) == 0)
                cout << "Control de red" << endl;

            if(traf[3] == '0')
                cout << "\t\t\tRetardo normal" << endl;
            else
                cout << "\t\t\tRetardo bajo" << endl;
            if(traf[4] == '0')
                cout << "\t\t\tRendimiento normal" << endl;
            else
                cout << "\t\t\tRendimiento alto" << endl;
            if(traf[5] == '0')
                cout << "\t\t\tFiabilidad normal" << endl;
            else
                cout << "\t\t\tFiabilidad alta" << endl;

            /**  Etiqueta de flujo  **/
            strncpy(byte3, &bits[4], 5);
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte3, byte2);
            cout << endl << byte3 ;
            unsigned long etiqueta = bitset<20>(byte3).to_ulong();
            cout << "\tEtiqueta de flujo: " << dec << etiqueta << endl;

            /**  TamaÃ±o de datos  **/
            strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
            strcat(byte2, bitset<8>(file.get()).to_string().c_str());
            cout << endl << byte2 ;
            unsigned long tamano_total = bitset<16>(byte2).to_ulong();
            cout << "\tTamanio de datos: " << dec << tamano_total << endl;

            /** Encabezado siguiente **/
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            cout << endl << bits;
            unsigned long encabezado = bitset<16>(bits).to_ulong();
            cout << "\t\tEncabezado siguiente: (" << dec << encabezado << ") ";
            if(encabezado == 1)
                cout<<"ICMPv4"<<endl;
            else if(encabezado == 6)
                cout<<"TCP"<<endl;
            else if(encabezado == 17)
                cout<<"UDP"<<endl;
            else if(encabezado == 58)
                cout<<"ICMPv6"<<endl;
            else if(encabezado == 118)
                cout<<"STP"<<endl;
            else if(encabezado == 121)
                cout<<"SMP"<<endl;
            else
                cout<<"Encabezado no encontrado"<<endl;

            /**LÃ­mite de saltos**/
            strcpy(bits, bitset<8>(file.get()).to_string().c_str());
            cout << endl << bits;
            unsigned long lim_salto = bitset<16>(bits).to_ulong();
            cout << "\t\tLimite de saltos: " << dec << lim_salto << endl;

            /**DirecciÃ³n de origen**/
            unsigned long ipOrigen;
            cout << endl << "\t\t\tDireccion IP-Origen: ";
            for(int i = 0; i < 8; i++){
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                ipOrigen = bitset<16>(byte2).to_ulong();
                cout << hex << ipOrigen;
                if(i < 7) cout << ":";
            }

            /**DirecciÃ³n de destino**/
            unsigned long ipDestino;
            cout << endl << "\t\t\tDireccion IP-Destino: ";
            for(int i = 0; i < 8; i++){
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                ipDestino = bitset<16>(byte2).to_ulong();
                cout << hex << ipDestino;
                if(i < 7) cout << ":";
            }
            /**Datos**/
            /**int cabeceraIpv6[40];
            cout << endl << "\nDatos: ";
            for(i = 14; i < 54; i++){
                cabeceraIpv6[i] = file.get();
                cout << hex << cabeceraIpv6[i] << " ";
            }**/

            /**   ---ICMPv6---   **/
            if(encabezado == 58){
                 cout << endl << "\n\n------                                          ICMPv6                                       -------" << endl;
                /**  Tipo  **/
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                unsigned long tipoICMP = bitset<8>(byte2).to_ulong();
                cout <<  "\t\tTipo: " << "(" << dec << tipoICMP << ") ";
                if(tipoICMP == 1)
                    cout << "Mensaje de destino inalcanzable" << endl;
                else if(tipoICMP == 2)
                    cout << "Mensaje de paquete demasiado grande" << endl;
                else if(tipoICMP == 3)
                    cout << "Time Exceeded Message" << endl;
                else if(tipoICMP == 4)
                    cout << "Mensaje de problema de parÃ¡metro" << endl;
                else if(tipoICMP == 128)
                    cout << "Mensaje perdido de eco" << endl;
                else if(tipoICMP == 129)
                    cout << "Mensaje de respuesta de eco" << endl;
                else if(tipoICMP == 133)
                    cout << "Mensaje de solicitud del router" << endl;
                else if(tipoICMP == 134)
                    cout << "Mensaje de anuncio del router" << endl;
                else if(tipoICMP == 135)
                    cout << "Mensaje de solicitud vecino" << endl;
                else if(tipoICMP == 136)
                    cout << "Mensaje de anuncio de vecino" << endl;
                else if(tipoICMP == 137)
                    cout << "Reoriente el mensaje" << endl;

                /**  Code  **/
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl <<  byte2;
                unsigned long code = bitset<8>(byte2).to_ulong();
                cout << "\t\tCodigo de error: " << "\n\t\t\t\t(" << dec << code << ") ";
                if((tipoICMP==2)||(tipoICMP==128)||(tipoICMP==129)||(tipoICMP==133)||(tipoICMP==134)||(tipoICMP==135)||(tipoICMP==136)||(tipoICMP==137))
                {
                    cout << code << endl;
                }
                else if(tipoICMP == 1)
                        {
                            if(code == 0)
                                cout << "No existe ruta destino" << endl;
                            else if(code == 1)
                                cout << "Comunicacion con el destino administrativamente prohibida" << endl;
                            else if(code == 2)
                                cout << "No asignado" << endl;
                            else if(code == 3)
                                cout << "Direccion inalcanzable" << endl;
                        }
                else if(tipoICMP == 3)
                        {
                            if(code == 0)
                                cout << "Limite del salto excedido" << endl;
                            else if(code == 1)
                                cout << "Tiempo e reensamble de fragmento excedido" << endl;
                        }
                else if(tipoICMP == 4)
                        {
                            if(code == 0)
                                cout << "El campo del encabezado erroneo encontro" << endl;
                            else if(code == 1)
                                cout << "Se encontro el siguiente tipo de encabezado desconocido" << endl;
                            else if(code == 2)
                                cout << "Opcion desconocida del IPv6 encontrada" << endl;
                        }
                
                /** Checksum  **/
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                lim_salto = bitset<16>(byte2).to_ulong();
                cout << "\tChecksum: " << hex << lim_salto << endl;

                /**Datos**/
                int cabeceraIpv6[40];
                cout << endl << "\nDatos: ";
                for(i = 14; i < 54; i++){
                    cabeceraIpv6[i] = file.get();
                    cout << hex << cabeceraIpv6[i] << " ";
                }
            }
            /**   ---TCP---   **/
            if(encabezado == 6){
                /**  Tipo  **/
                cout << endl << "\n\n------                                            TCP                                        -------" << endl;
                /**  Puerto Origen  **/
                unsigned long pOrigen;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                pOrigen = bitset<16>(byte2).to_ulong();
                cout << "\tPuerto Origen: (" << dec << pOrigen << ") ";
                if(pOrigen >= 0 && pOrigen <= 1023){
                    cout << " Puertos bien conocidos" << endl << "\t\t\t";
                    if(pOrigen == 20){
                        cout << "Puerto: 20" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 21){
                        cout << "Puerto: 21" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 22){
                        cout << "Puerto: 22" << endl << "\t\t\tServicio: SSH" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 23){
                        cout << "Puerto: 23" << endl << "\t\t\tServicio: TELNET" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 25){
                        cout << "Puerto: 25" << endl << "\t\t\tServicio: SMTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 53)
                    {
                        cout << "Puerto: 53" << endl << "\t\t\tServicio: DNS" << endl << "\t\t\tProtocolo: TCP/UDP" << endl;
                    }
                    else if(pOrigen == 67){
                        cout << "Puerto: 67" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 68){
                        cout << "Puerto: 68" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 69){
                        cout << "Puerto: 69" << endl << "\t\t\tServicio: TFTP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 80){
                        cout << "Puerto: 80" << endl << "\t\t\tServicio: HTTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 110){
                        cout << "Puerto: 110" << endl << "\t\t\tServicio: POP3" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 143){
                        cout << "Puerto: 143" << endl << "\t\t\tServicio: IMAP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 443){
                        cout << "Puerto: 443" << endl << "\t\t\t\t\t\tServicio: HTTPS" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 993){
                        cout << "Puerto: 993" << endl << "\t\t\tServicio: IMAP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 995){
                        cout << "Puerto: 995" << endl << "\t\t\tServicio: POP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                }
                else if(pOrigen >= 1024 && pOrigen <= 49151)
                    cout << " Puertos registrados" << endl;
                else if(pOrigen >= 49151 && pOrigen <= 65535)
                    cout << " Puertos dinamicos o privados" << endl;

                /**  Puerto Destino  **/
                unsigned long pDestino;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                pDestino = bitset<16>(byte2).to_ulong();
                cout << "\tPuerto Destino: (" << dec << pDestino << ") ";
                if(pDestino >= 0 && pDestino <= 1023){
                    cout << " Puertos bien conocidos" << endl << "\t\t\t";
                    if(pDestino == 20){
                        cout << "Puerto: 20" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 21){
                        cout << "Puerto: 21" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 22){
                        cout << "Puerto: 22" << endl << "\t\t\tServicio: SSH" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 23){
                        cout << "Puerto: 23" << endl << "\t\t\tServicio: TELNET" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 25){
                        cout << "Puerto: 25" << endl << "\t\t\tServicio: SMTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 53){
                        cout << "Puerto: 53" << endl << "\t\t\tServicio: DNS" << endl << "\t\t\tProtocolo: TCP/UDP" << endl;
                    }
                    else if(pDestino == 67){
                        cout << "Puerto: 67" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 68){
                        cout << "Puerto: 68" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 69){
                        cout << "Puerto: 69" << endl << "\t\t\tServicio: TFTP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 80){
                        cout << "Puerto: 80" << endl << "\t\t\tServicio: HTTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 110){
                        cout << "Puerto: 110" << endl << "\t\t\tServicio: POP3" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 143){
                        cout << "Puerto: 143" << endl << "\t\t\tServicio: IMAP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 443){
                        cout << "Puerto: 443" << endl << "\t\t\tServicio: HTTPS" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 993){
                        cout << "Puerto: 993" << endl << "\t\t\tServicio: IMAP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 995){
                        cout << "Puerto: 995" << endl << "\t\t\tServicio: POP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                }
                else if(pDestino >= 1024 && pDestino <= 49151)
                    cout << " Puertos registrados" << endl;
                else if(pDestino >= 49151 && pDestino <= 65535)
                    cout << " Puertos dinamicos o privados" << endl;   
            

                /**  Número de secuencia  **/
                unsigned long num_secuencia;
                strcpy(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte3;
                num_secuencia = bitset<32>(byte3).to_ulong();
                cout << "\tNumero de secuencia: " << dec << num_secuencia;

                /**  Número de acuse de recibo  **/
                unsigned long num_acuse;
                strcpy(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte3;
                num_acuse = bitset<32>(byte3).to_ulong();
                cout << "\tNumero de acuse de recibo: " << dec << num_acuse << endl;

                /**  Longitud de Cabecera  **/
                char ihl[5];
                strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                strncpy(ihl, bits, 4); 
                string tamano = ihl;
                unsigned long palabrastcp = bitset<8>(tamano).to_ulong();
                cout << endl << ihl << "\t\t\tLongitud de cabecera: " << dec << palabrastcp << " palabras, " << dec << palabrastcp*32 << " bits" << endl;

                /**  Reservado  **/
                for(i = 0; i < 4; i++){
                    bits[i] = bits[i+4];
                }
              
                char reservadotcp[4] = " ";
                strncpy(reservadotcp, bits, 3);
                cout << endl << reservadotcp;
                cout << "\t\t\tReservado: " << reservadotcp;

                /**  Banderas  **/
                string b;
                b = bits[3];
                unsigned long band1 = bitset<1>(b).to_ulong();
                cout << endl << endl << "\t\t\tBanderas de comunicacion de TCP: " << endl;
                if(band1 == 1){
                    cout << "\t\t\t\tNS:  (" << band1 << ") ECN-nonce concealment protection." << endl;
                }
                strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                b = bits[0];
                unsigned long band2 = bitset<1>(b).to_ulong();
                if(band2 == 1){
                    cout << "\t\t\t\tCWR: (" << band2 << ") Congestion Window Reduced." << endl;
                }
                b = bits[1];
                unsigned long band3 = bitset<1>(b).to_ulong();
                if(band3 == 1){
                    cout << "\t\t\t\tECE: (" << band3 << ") Indicaciones sobre congestion." << endl;
                }
                b = bits[2];
                unsigned long band4 = bitset<1>(b).to_ulong();
                if(band4 == 1){
                    cout << "\t\t\t\tURG: (" << band4 << ") Campo de puntero urgente es valido." << endl;
                }
                b = bits[3];
                unsigned long band5 = bitset<1>(b).to_ulong();
                if(band5 == 1){
                    cout << "\t\t\t\tACK: (" << band5 << ") Acknowledge." << endl;
                }
                b = bits[4];
                unsigned long band6 = bitset<1>(b).to_ulong();
                if(band6 == 1){
                    cout << "\t\t\t\tPHS: (" << band6 << ") Push." << endl;
                }
                b = bits[5];
                unsigned long band7 = bitset<1>(b).to_ulong();
                if(band7 == 1){
                    cout << "\t\t\t\tRST: (" << band7 << ") Reset." << endl;
                }
                b = bits[6];
                unsigned long band8 = bitset<1>(b).to_ulong();
                if(band8 == 1){
                    cout << "\t\t\t\tSYN: (" << band8 << ") Synchronize." << endl;
                }
                b = bits[7];
                unsigned long band9 = bitset<1>(b).to_ulong();
                if(band9 == 1){
                    cout << "\t\t\t\tFIN: (" << band9 << ") Finish." << endl;
                }

                /**  Tamaño de ventana de recepción  **/
                unsigned long tamano_ventana;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                tamano_ventana = bitset<16>(byte2).to_ulong();
                cout << "\tTamano de ventana: " << dec << tamano_ventana;
           
                /**  Suma de verificación  **/
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                unsigned long checksum = bitset<16>(byte2).to_ulong();
                cout << "\tChecksum: " << hex << checksum << endl;

                /**  Puntero urgente  **/
                unsigned long puntero_urgente;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                puntero_urgente = bitset<16>(byte2).to_ulong();
                cout << puntero_urgente << "\t\t\tPuntero urgente: " << dec << puntero_urgente;

                /**  Datos  **/
                int cabeceratcp[20];
                cout << endl << "\n\nDatos: ";
                for(i = 54; i < 74; i++){
                    cabeceratcp[i] = file.get();
                    cout << hex << cabeceratcp[i] << " ";
                }

                /**   ---DNS---   **/
                if(pDestino == 53){
                    /**  Tipo  **/
                    cout << endl << "\n\n------                                       Servicio DNS                                    -------" << endl;
                    /** ID **/
                    char OpCode[5] = " ", z[4] = " ", Rcode[5] = " ";
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << endl << byte2;
                    unsigned long ID = bitset<16>(byte2).to_ulong();
                    cout << "\tID: " << hex << ID << endl;

                    /** Banderas **/
                    cout << endl << "\t\t\tBanderas" << endl;
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << "\t\t\t\tQR: ";
                    if(byte2[0] == '0')
                        cout << "(0) Consulta" << endl;
                    else
                        cout << "(1) Respuesta" << endl;
                    strncpy(OpCode, &byte2[1], 4);
                    cout << OpCode;
                    cout<<"\t\t\t\tOp Code: ";
                    if(strcmp(OpCode, "0000") == 0)
                        cout << "(0) Consulta estandar(QUERY)" << endl;
                    else if(strcmp(OpCode, "0001") == 0)
                        cout << "(1) Consulta inversa(IQUERY)" << endl;
                    else if(strcmp(OpCode, "0010") == 0)
                        cout << "(2) solicitud del estado del servidor(STATUS)" << endl;
                    else
                        cout << "Reservados" <<endl;
                    if(byte2[5] == '0')
                        cout << "\t\t\t\tAA(0) No tiene respuesta autoritativa." << endl;
                    else
                        cout << "\t\t\t\tAA(1) Respuesta Autoritativa." << endl;
                    if(byte2[6] == '0')
                        cout << "\t\t\t\tTC(0) Respuesta no truncada." << endl;
                    else
                        cout << "\t\t\t\tTC(1) Respuesta truncada." << endl;
                    if(byte2[7] == '0')
                        cout << "\t\t\t\tRD(0) Recursion no deseada." << endl;
                    else
                        cout << "\t\t\t\tRD(1) Recursion deseada." << endl;
                    if(byte2[8] == '0')
                        cout << "\t\t\t\tRA(0) Recursividad no disponible." << endl;
                    else
                        cout << "\t\t\t\tRA(1) Recursividad Disponible." << endl;
                        
                    strncpy(z, &byte2[9], 3);
                    cout << z;
                    cout << "\t\t\t\tZ (Reservados)" << endl;
                    strncpy(Rcode, &byte2[12], 4);
                    cout << Rcode;
                    if(strcmp(Rcode, "0000") == 0)
                        cout << "\t\t\t\t(0) Ningun error." << endl;
                    else if(strcmp(Rcode, "0001") == 0)
                        cout << "\t\t\t\t(1) Error de formato." << endl;
                    else if(strcmp(Rcode, "0010") == 0)
                        cout << "\t\t\t\t(2) Fallo en el servidor." << endl;
                    else if(strcmp(Rcode, "0011") == 0)
                        cout << "\t\t\t\t(3) Error en nombre." << endl;
                    else if(strcmp(Rcode, "0100") == 0)
                        cout << "\t\t\t\t(4) No implementado." << endl;
                    else if(strcmp(Rcode, "0101") == 0)
                        cout << "\t\t\t\t(5) Rechazado." << endl;
                    else
                        cout << "Reservados." <<endl;

                    /** Contadores **/
                    cout << endl << "\t\t\tContadores" << endl;
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout  << byte2;
                    unsigned long QD = bitset<16>(byte2).to_ulong();
                    cout << "\t\tQDcount: (" << dec << QD << ") Preguntas" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long AN = bitset<16>(byte2).to_ulong();
                    cout << "\t\tANcount: (" << dec << AN << ") Respuestas" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long NS = bitset<16>(byte2).to_ulong();
                    cout << "\t\tNScount: (" << dec << NS << ") Autoridad" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long AR = bitset<16>(byte2).to_ulong();
                    cout << "\t\tARcount: (" << dec << AR << ") Registros adicionales" << endl << endl;

                    /**Campos de Pregunta**/
                    unsigned long puntero, numLetras, letra, tipoDNS, claseDNS;
                    char ascii;
                    cout << endl << "\t\t\tCampo de Pregunta" << endl;
                    cout << "Nombre del dominio: \t";
                    for(int i = 0; i < QD; i++){
                      numLetras = bitset<8>(file.get()).to_ulong();
                      do
                      {
                        cout << "<" << dec << numLetras << "><";
                        for(int i = 0; i < numLetras; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << ">";
                        numLetras = bitset<8>(file.get()).to_ulong();
                      }while(numLetras != 0);

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      tipoDNS = bitset<16>(byte2).to_ulong();
                      cout << endl << byte2;
                      cout << "\t\tTipo: ";
                      if(tipoDNS == 1)
                        cout << "A (1)"; 
                      else if(tipoDNS == 5)
                        cout << "CNAME (5)";
                      else if(tipoDNS == 13)
                        cout << "HINFO (13)";
                      else if(tipoDNS == 15)                           cout << "MX (15)";
                      else if(tipoDNS == 22 || tipoDNS == 23)
                        cout << "NS (22)";
                      else
                        cout << "No encontrado";

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      cout << "\t\tClase: ";
                      claseDNS = bitset<16>(byte2).to_ulong();
                      if(claseDNS == 1)
                        cout << "IN (1)" << endl; 
                      else if(claseDNS == 3)
                        cout << "CH (3)" << endl;
                      else
                        cout << "No encontrado" << endl;
                    }

                    /** Campos de respuesta **/
                    for(int i = 0; i < AN; i++){
                      cout << endl << "\t\t\tCampo de Respuesta" << endl;
                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      puntero = bitset<16>(byte2).to_ulong();
                      cout << "\t\tPTR: "<< hex << puntero << endl;

                      file.seekg(puntero);
                      numLetras = bitset<8>(file.get()).to_ulong();
                      cout << "Nombre del dominio: \t";
                      do
                      {
                        cout << "<" << dec << numLetras << "><";
                        for(int i = 0; i < numLetras; i++)
                        {
                          letra =  bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << ">";
                        numLetras = bitset<8>(file.get()).to_ulong();
                      } while(numLetras != 0);

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      tipoDNS = bitset<16>(byte2).to_ulong();
                      cout << endl << byte2;
                      cout << "\t\tTipo: ";
                      if(tipoDNS == 1)
                        cout << "A (1)"; 
                      else if(tipoDNS == 5)
                        cout << "CNAME (5)";
                      else if(tipoDNS == 13)
                        cout << "HINFO (13)";
                      else if(tipoDNS == 15)                           cout << "MX (15)";
                      else if(tipoDNS == 22 || tipoDNS == 23)
                        cout << "NS";
                      else
                        cout << "No encontrado";

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      cout << "\t\tClase: ";
                      claseDNS = bitset<16>(byte2).to_ulong();
                      if(claseDNS == 1)
                        cout << "IN (1)" << endl; 
                      else if(claseDNS == 3)
                        cout << "CH (3)" << endl;
                      else
                        cout << "No encontrado" << endl;

                      strcpy(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      unsigned long TTL = bitset<32>(byte3).to_ulong();
                      cout << "TTL: " << dec << TTL << endl;

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      unsigned long longDatos;
                      cout << "Logitud de datos: " << dec << longDatos << endl;

                      unsigned long Rdata;
                      cout << endl << "\n\t\t\tRDATA: ";
                      if(tipoDNS == 1){ // A
                        cout << endl << "\tDireccion IP: ";
                        for(int i = 0; i < 4; i++){
                          strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                          Rdata = bitset<8>(bits).to_ulong();
                          cout << dec << Rdata;
                          if(i < 3) cout << ".";
                        }
                      }
                      else if(tipoDNS == 5){ // CNAME
                        for(int i = 0; i < AN; i++){
                          numLetras = bitset<8>(file.get()).to_ulong();
                          cout << endl << "\tNombre del dominio: ";
                          do
                          {
                            for(int i = 0; i < numLetras; i++)
                            {
                              letra =  bitset<8>(file.get()).to_ulong();
                              ascii = letra;
                              cout << ascii;
                            }
                            cout << ".";
                            numLetras = bitset<8>(file.get()).to_ulong();
                          } while(numLetras != 0);
                        }
                      }
                      else if(tipoDNS == 15){ // MX
                        unsigned long prioridad;
                        strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                        strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                        cout << endl << byte2;
                        prioridad = bitset<16>(byte2).to_ulong();
                        cout << "\tPrioridad: " << prioridad;
                        cout << endl << "\tNombre del ordenador: ";
                        for(int i = 0; i < longDatos - 2; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << endl;
                      }
                      else if(tipoDNS == 22 || tipoDNS == 23){ // NS
                        cout << endl << "\tNombre del ordenador: ";
                        for(int i = 0; i < longDatos; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                      }
                    }
                }
            }
            /**   ---UDP---   **/
            if(encabezado == 17){
                /**  Tipo  **/
                cout << endl << "\n\n------                                            UDP                                        -------" << endl;
                /**  Puerto Origen  **/
                unsigned long pOrigen;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                pOrigen = bitset<16>(byte2).to_ulong();
                cout << "\tPuerto Origen: (" << dec << pOrigen << ") ";
                if(pOrigen >= 0 && pOrigen <= 1023){
                    cout << " Puertos bien conocidos" << endl << "\t\t\t";
                    if(pOrigen == 20){
                        cout << "Puerto: 20" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 21){
                        cout << "Puerto: 21" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 22){
                        cout << "Puerto: 22" << endl << "\t\t\tServicio: SSH" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 23){
                        cout << "Puerto: 23" << endl << "\t\t\tServicio: TELNET" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 25){
                        cout << "Puerto: 25" << endl << "\t\t\tServicio: SMTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 53){
                        cout << "Puerto: 53" << endl << "\t\t\tServicio: DNS" << endl << "\t\t\tProtocolo: TCP/UDP" << endl;
                    }
                    else if(pOrigen == 67){
                        cout << "Puerto: 67" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 68){
                        cout << "Puerto: 68" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 69){
                        cout << "Puerto: 69" << endl << "\t\t\tServicio: TFTP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pOrigen == 80){
                        cout << "Puerto: 80" << endl << "\t\t\tServicio: HTTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 110){
                        cout << "Puerto: 110" << endl << "\t\t\tServicio: POP3" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 143){
                        cout << "Puerto: 143" << endl << "\t\t\tServicio: IMAP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 443){
                        cout << "Puerto: 443" << endl << "\t\t\tServicio: HTTPS" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 993){
                        cout << "Puerto: 993" << endl << "\t\t\tServicio: IMAP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pOrigen == 995){
                        cout << "Puerto: 995" << endl << "\t\t\tServicio: POP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                }
                else if(pOrigen >= 1024 && pOrigen <= 49151)
                    cout << " Puertos registrados" << endl;
                else if(pOrigen >= 49151 && pOrigen <= 65535)
                    cout << " Puertos dinamicos o privados" << endl;

                /**  Puerto Destino  **/
                unsigned long pDestino;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                pDestino = bitset<16>(byte2).to_ulong();
                cout << "\tPuerto Destino: (" << dec << pDestino << ") ";
                if(pDestino >= 0 && pDestino <= 1023){
                    cout << " Puertos bien conocidos" << endl << "\t\t\t";
                    if(pDestino == 20){
                        cout << "Puerto: 20" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 21){
                        cout << "Puerto: 21" << endl << "\t\t\tServicio: FTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 22){
                        cout << "Puerto: 22" << endl << "\t\t\tServicio: SSH" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 23){
                        cout << "Puerto: 23" << endl << "\t\t\tServicio: TELNET" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 25){
                        cout << "Puerto: 25" << endl << "\t\t\tServicio: SMTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 53){
                        cout << "Puerto: 53" << endl << "\t\t\tServicio: DNS" << endl << "\t\t\tProtocolo: TCP/UDP" << endl;
                    }
                    else if(pDestino == 67){
                        cout << "Puerto: 67" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 68){
                        cout << "Puerto: 68" << endl << "\t\t\tServicio: DHCP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 69){
                        cout << "Puerto: 69" << endl << "\t\t\tServicio: TFTP" << endl << "\t\t\tProtocolo: UDP" << endl;
                    }
                    else if(pDestino == 80){
                        cout << "Puerto: 80" << endl << "\t\t\tServicio: HTTP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 110){
                        cout << "Puerto: 110" << endl << "\t\t\tServicio: POP3" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 143){
                        cout << "Puerto: 143" << endl << "\t\t\tServicio: IMAP" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 443){
                        cout << "Puerto: 443" << endl << "\t\t\tServicio: HTTPS" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 993){
                        cout << "Puerto: 993" << endl << "\t\t\tServicio: IMAP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                    else if(pDestino == 995){
                        cout << "Puerto: 995" << endl << "\t\t\tServicio: POP SSL" << endl << "\t\t\tProtocolo: TCP" << endl;
                    }
                }
                else if(pDestino >= 1024 && pDestino <= 49151)
                    cout << " Puertos registrados" << endl;
                else if(pDestino >= 49151 && pDestino <= 65535)
                    cout << " Puertos dinamicos o privados" << endl;   

                /**  Longitud total  **/
                unsigned long longitud_total;
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                longitud_total = bitset<16>(byte2).to_ulong();
                cout << "\tLongitud Total: " << dec << longitud_total;

                /**  Suma de verificación  **/
                strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                cout << endl << byte2;
                unsigned long checksum = bitset<16>(byte2).to_ulong();
                cout << "\tChecksum: " << hex << checksum;

                /**  Datos  **/
                int cabeceraudp[8];
                cout << endl << "\n\nDatos: ";
                for(i = 54; i < 62; i++){
                    cabeceraudp[i] = file.get();
                    cout << hex << cabeceraudp[i] << " ";
                }
                
                /**   ---DNS---   **/
                if(pDestino == 53){
                    /**  Tipo  **/
                    cout << endl << "\n\n------                                       Servicio DNS                                    -------" << endl;
                    /** ID **/
                    char OpCode[5] = " ", z[4] = " ", Rcode[5] = " ";
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << endl << byte2;
                    unsigned long ID = bitset<16>(byte2).to_ulong();
                    cout << "\tID: " << hex << ID << endl;

                    /** Banderas **/
                    cout << endl << "\t\t\tBanderas" << endl;
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << "\t\t\t\tQR: ";
                    if(byte2[0] == '0')
                        cout << "(0) Consulta" << endl;
                    else
                        cout << "(1) Respuesta" << endl;
                    strncpy(OpCode, &byte2[1], 4);
                    cout << OpCode;
                    cout<<"\t\t\t\tOp Code: ";
                    if(strcmp(OpCode, "0000") == 0)
                        cout << "(0) Consulta estandar(QUERY)" << endl;
                    else if(strcmp(OpCode, "0001") == 0)
                        cout << "(1) Consulta inversa(IQUERY)" << endl;
                    else if(strcmp(OpCode, "0010") == 0)
                        cout << "(2) solicitud del estado del servidor(STATUS)" << endl;
                    else
                        cout << "Reservados" <<endl;
                    if(byte2[5] == '0')
                        cout << "\t\t\t\tAA(0) No tiene respuesta autoritativa." << endl;
                    else
                        cout << "\t\t\t\tAA(1) Respuesta Autoritativa." << endl;
                    if(byte2[6] == '0')
                        cout << "\t\t\t\tTC(0) Respuesta no truncada." << endl;
                    else
                        cout << "\t\t\t\tTC(1) Respuesta truncada." << endl;
                    if(byte2[7] == '0')
                        cout << "\t\t\t\tRD(0) Recursion no deseada." << endl;
                    else
                        cout << "\t\t\t\tRD(1) Recursion deseada." << endl;
                    if(byte2[8] == '0')
                        cout << "\t\t\t\tRA(0) Recursividad no disponible." << endl;
                    else
                        cout << "\t\t\t\tRA(1) Recursividad Disponible." << endl;
                        
                    strncpy(z, &byte2[9], 3);
                    cout << z;
                    cout << "\t\t\t\tZ (Reservados)" << endl;
                    strncpy(Rcode, &byte2[12], 4);
                    cout << Rcode;
                    if(strcmp(Rcode, "0000") == 0)
                        cout << "\t\t\t\t(0) Ningun error." << endl;
                    else if(strcmp(Rcode, "0001") == 0)
                        cout << "\t\t\t\t(1) Error de formato." << endl;
                    else if(strcmp(Rcode, "0010") == 0)
                        cout << "\t\t\t\t(2) Fallo en el servidor." << endl;
                    else if(strcmp(Rcode, "0011") == 0)
                        cout << "\t\t\t\t(3) Error en nombre." << endl;
                    else if(strcmp(Rcode, "0100") == 0)
                        cout << "\t\t\t\t(4) No implementado." << endl;
                    else if(strcmp(Rcode, "0101") == 0)
                        cout << "\t\t\t\t(5) Rechazado." << endl;
                    else
                        cout << "Reservados." <<endl;

                    /** Contadores **/
                    cout << endl << "\t\t\tContadores" << endl;
                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout  << byte2;
                    unsigned long QD = bitset<16>(byte2).to_ulong();
                    cout << "\t\tQDcount: (" << dec << QD << ") Preguntas" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long AN = bitset<16>(byte2).to_ulong();
                    cout << "\t\tANcount: (" << dec << AN << ") Respuestas" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long NS = bitset<16>(byte2).to_ulong();
                    cout << "\t\tNScount: (" << dec << NS << ") Autoridad" << endl;

                    strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                    strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                    cout << byte2;
                    unsigned long AR = bitset<16>(byte2).to_ulong();
                    cout << "\t\tARcount: (" << dec << AR << ") Registros adicionales" << endl << endl;

                    /**Campos de Pregunta**/
                    unsigned long puntero, numLetras, letra, tipoDNS, claseDNS;
                    char ascii;
                    cout << endl << "\t\t\tCampo de Pregunta" << endl;
                    cout << "Nombre del dominio: \t";
                    for(int i = 0; i < QD; i++){
                      numLetras = bitset<8>(file.get()).to_ulong();
                      do
                      {
                        cout << "<" << dec << numLetras << "><";
                        for(int i = 0; i < numLetras; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << ">";
                        numLetras = bitset<8>(file.get()).to_ulong();
                      }while(numLetras != 0);

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      tipoDNS = bitset<16>(byte2).to_ulong();
                      cout << endl << byte2;
                      cout << "\t\tTipo: ";
                      if(tipoDNS == 1)
                        cout << "A (1)"; 
                      else if(tipoDNS == 5)
                        cout << "CNAME (5)";
                      else if(tipoDNS == 13)
                        cout << "HINFO (13)";
                      else if(tipoDNS == 15)                           cout << "MX (15)";
                      else if(tipoDNS == 22 || tipoDNS == 23)
                        cout << "NS (22)";
                      else
                        cout << "No encontrado";

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      cout << "\t\tClase: ";
                      claseDNS = bitset<16>(byte2).to_ulong();
                      if(claseDNS == 1)
                        cout << "IN (1)" << endl; 
                      else if(claseDNS == 3)
                        cout << "CH (3)" << endl;
                      else
                        cout << "No encontrado" << endl;
                    }

                    /** Campos de respuesta **/
                    for(int i = 0; i < AN; i++){
                      cout << endl << "\t\t\tCampo de Respuesta" << endl;
                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      puntero = bitset<16>(byte2).to_ulong();
                      cout << "\t\tPTR: "<< hex << puntero << endl;

                      file.seekg(puntero);
                      numLetras = bitset<8>(file.get()).to_ulong();
                      cout << "Nombre del dominio: \t";
                      do
                      {
                        cout << "<" << dec << numLetras << "><";
                        for(int i = 0; i < numLetras; i++)
                        {
                          letra =  bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << ">";
                        numLetras = bitset<8>(file.get()).to_ulong();
                      } while(numLetras != 0);

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      tipoDNS = bitset<16>(byte2).to_ulong();
                      cout << endl << byte2;
                      cout << "\t\tTipo: ";
                      if(tipoDNS == 1)
                        cout << "A (1)"; 
                      else if(tipoDNS == 5)
                        cout << "CNAME (5)";
                      else if(tipoDNS == 13)
                        cout << "HINFO (13)";
                      else if(tipoDNS == 15)                           cout << "MX (15)";
                      else if(tipoDNS == 22 || tipoDNS == 23)
                        cout << "NS";
                      else
                        cout << "No encontrado";

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      cout << endl << byte2;
                      cout << "\t\tClase: ";
                      claseDNS = bitset<16>(byte2).to_ulong();
                      if(claseDNS == 1)
                        cout << "IN (1)" << endl; 
                      else if(claseDNS == 3)
                        cout << "CH (3)" << endl;
                      else
                        cout << "No encontrado" << endl;

                      strcpy(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte3, bitset<8>(file.get()).to_string().c_str());
                      unsigned long TTL = bitset<32>(byte3).to_ulong();
                      cout << "TTL: " << dec << TTL << endl;

                      strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                      strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                      unsigned long longDatos;
                      cout << "Logitud de datos: " << dec << longDatos << endl;

                      unsigned long Rdata;
                      cout << endl << "\n\t\t\tRDATA: ";
                      if(tipoDNS == 1){ // A
                        cout << endl << "\tDireccion IP: ";
                        for(int i = 0; i < 4; i++){
                          strcpy(bits, bitset<8>(file.get()).to_string().c_str());
                          Rdata = bitset<8>(bits).to_ulong();
                          cout << dec << Rdata;
                          if(i < 3) cout << ".";
                        }
                      }
                      else if(tipoDNS == 5){ // CNAME
                        for(int i = 0; i < AN; i++){
                          numLetras = bitset<8>(file.get()).to_ulong();
                          cout << endl << "\tNombre del dominio: ";
                          do
                          {
                            for(int i = 0; i < numLetras; i++)
                            {
                              letra =  bitset<8>(file.get()).to_ulong();
                              ascii = letra;
                              cout << ascii;
                            }
                            cout << ".";
                            numLetras = bitset<8>(file.get()).to_ulong();
                          } while(numLetras != 0);
                        }
                      }
                      else if(tipoDNS == 15){ // MX
                        unsigned long prioridad;
                        strcpy(byte2, bitset<8>(file.get()).to_string().c_str());
                        strcat(byte2, bitset<8>(file.get()).to_string().c_str());
                        cout << endl << byte2;
                        prioridad = bitset<16>(byte2).to_ulong();
                        cout << "\tPrioridad: " << prioridad;
                        cout << endl << "\tNombre del ordenador: ";
                        for(int i = 0; i < longDatos - 2; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                        cout << endl;
                      }
                      else if(tipoDNS == 22 || tipoDNS == 23){ // NS
                        cout << endl << "\tNombre del ordenador: ";
                        for(int i = 0; i < longDatos; i++)
                        {
                          letra = bitset<8>(file.get()).to_ulong();
                          ascii = letra;
                          cout << ascii;
                        }
                      }
                    }
                }
            }
        }
        else
            cout << "Tipo no encontrado";
    }
    file.close();
    cout << endl << endl;
    return 0;
}