# Sistema de Reserva de Voos

## Descrição
Sistema feito em C, Utilizando o toolkit GTK para moldar a interface gráfica (GUI).
Criado como projeto final da matéria ALGORITMOS, Módulo 1 do curso de ADS - IFPI CACOR, segue as guidelines presentes
no repositório como parametro da base funcional do sistema.
Funcionalidades bastante básicas, qualquer complexidade é culpa da GUI.

## Features

    - GUI usando GTK
    - PGO (Progamação Orientada a Gambiarra)
    - 0 modularidade (Não deu tempo...)
    - Comentários minímos!!!

## Compilando e Executando

Depende inteiramente do GTK para funcionar. 
Desenvolvido e testado utilizando o ambiente MSYS2(WINDOWS) com GCC e libs do GTK, também deve rodar em linux.

    - GTK no LINUX: https://www.gtk.org/docs/installations/linux/
    - GTK no WINDOWS: https://www.gtk.org/docs/installations/windows/
    - GTK no MACOS: melhore.

    - "prototype.c" é apenas um esboço de terminal criado para desenvolver a lógica do sistema, ignorável 
    - Navegue para o diretório dos arquivos (cd /c/users/Alguma_pasta_ai)
    - Comando para compilar: gcc $( pkg-config --cflags gtk4 ) -o main main.c $( pkg-config --libs gtk4 )
    - Comando para executar: ./main
