# Bomber...tale?

Um jogo de arena para dois jogadores, inspirado em **Bomberman** e com uma temática baseada em **Undertale**. Este projeto foi desenvolvido em C com a biblioteca **raylib** como trabalho final da disciplina de Algoritmos, no primeiro período da faculdade (2023/2).

## Sobre o jogo

Dois jogadores dividem a arena, posicionam bombas e tentam sobreviver às explosões enquanto atravessam obstáculos destrutíveis e disputam itens. Há dois mapas disponíveis, cada um com seu próprio cenário e disposição dos itens. Se nenhum jogador perder até o limite de dois minutos, a partida termina em empate.

Os itens são parte importante da estratégia: alguns melhoram atributos e outros os reduzem. **A aparência não revela o efeito**: os itens positivos e negativos usam o mesmo visual para cada tipo, então é preciso descobrir o resultado ao coletá-los.

## Recursos

- Partidas locais para dois jogadores no mesmo teclado.
- Dois mapas selecionáveis no menu inicial.
- Bombas com explosões que se propagam pelas quatro direções e são bloqueadas pelas barreiras sólidas.
- Caixas destrutíveis e itens de efeito positivo ou negativo.
- Personagens animados, cenários temáticos, música de menu e música durante a partida.
- Limite de partida de dois minutos, com tela de empate.

## Como jogar

Na tela inicial, digite os nomes dos jogadores, escolha um mapa e pressione **Enter** ou **Espaço** para começar. Use **Shift direito** para alternar entre os campos de nome. O campo aceita até seis caracteres; use **Backspace** para apagar.

| Ação | Jogador 1 | Jogador 2 |
|---|---|---|
| Mover | `W` `A` `S` `D` | `↑` `←` `↓` `→` |
| Colocar bomba | `Espaço` | `Enter` do teclado numérico |

Uma explosão pode atingir qualquer um dos jogadores, inclusive quem colocou a bomba. Ao fim da partida, use as setas e **Enter** para escolher a revanche ou fechar o jogo. Na tela de empate, as opções permitem voltar à tela inicial ou fechar.

## Itens

Os efeitos previstos são alterações em velocidade, quantidade de bombas disponíveis e alcance das explosões. Cada atributo pode receber um efeito positivo ou negativo. Como os dois tipos compartilham o mesmo ícone, não é possível distingui-los visualmente antes da coleta.

## Requisitos

- Compilador C, como GCC ou Clang.
- Biblioteca **raylib** instalada e configurada para o seu sistema operacional.
- Sistema com suporte gráfico e áudio compatível com a instalação da raylib.

## Instalar a raylib

A raylib é uma biblioteca C gratuita e de código aberto. A forma de instalar depende do sistema e do compilador que você pretende usar. As instruções oficiais são mantidas no [repositório da raylib](https://github.com/raysan5/raylib) e na [Wiki de instalação por plataforma](https://github.com/raysan5/raylib/wiki#development-platforms).

### Windows

Para começar, você pode usar o [pacote de instalação para Windows](https://raysan5.itch.io/raylib), que inclui um ambiente pré-configurado. Outra opção é instalar a raylib e o compilador separadamente, seguindo o guia oficial [Working on Windows](https://github.com/raysan5/raylib/wiki/Working-on-Windows). Use uma versão da raylib compatível com seu compilador (por exemplo, MinGW-w64/GCC ou Visual Studio) e configure os caminhos de inclusão e bibliotecas no comando de compilação.

### Linux

Instale um compilador C, `make`, `git` e as dependências gráficas e de áudio indicadas para sua distribuição no guia oficial [Working on GNU/Linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux). Em Ubuntu, por exemplo:

```bash
sudo apt install build-essential git libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
git clone --depth 1 https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

O comando acima compila e instala a biblioteca nos diretórios padrão do sistema. As dependências e os comandos mudam entre distribuições; consulte o guia oficial se estiver usando Fedora, Arch Linux, openSUSE ou outra distribuição.

### macOS

Com o [Homebrew](https://brew.sh/) instalado, instale a biblioteca e a ferramenta `pkg-config`:

```bash
brew install raylib pkgconf
```

O guia oficial [Working on macOS](https://github.com/raysan5/raylib/wiki/Working-on-macOS) traz outras opções de instalação e configuração.

## Compilar e executar

Os arquivos de mídia são carregados por caminhos relativos. Execute o jogo a partir da pasta que contém `main.c` e os arquivos `.png` e `.mp3`.

Em Linux, com a raylib instalada e seus pacotes de desenvolvimento disponíveis:

```bash
cc main.c -o bomber-tale -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
./bomber-tale
```

As bibliotecas de janela e gráficos podem variar conforme a distribuição. No Windows e no macOS, configure os caminhos de inclusão e bibliotecas da raylib conforme a instalação local. O ZIP original também inclui um executável chamado `main`, mas ele pode não ser compatível com o seu sistema.

## Arquivos do projeto

```text
.
├── main.c
├── main                    # executável incluído no ZIP original
├── README.md
├── doc/
│   └── documentação.pdf
├── *.png                   # personagens, cenários, barreiras e itens
└── *.mp3                   # trilhas de áudio
```

## Contexto e créditos

Este é um projeto acadêmico e de fã, sem vínculo oficial com os criadores ou detentores dos direitos de **Undertale**. Os personagens, músicas e demais materiais de terceiros pertencem aos respectivos titulares. Antes de redistribuir esses arquivos em um repositório público, confirme as permissões e inclua os créditos ou avisos exigidos por suas licenças.

O projeto não declara uma licença de código. Se quiser permitir que outras pessoas reutilizem o código, escolha e adicione uma licença apropriada ao repositório.
