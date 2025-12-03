FROM ubuntu:24.04

# Install Development APT Packages
RUN --mount=type=cache,target=/var/cache/apt \
    apt -qq update && \
    apt -qq install -y --no-install-recommends \
        # general
        apt-utils \
        lsb-release \
        # building
        mingw-w64 \
        build-essential \
        software-properties-common \
        ninja-build \
         # Python (For aqtinstall)
        python3 python3-pip python3-venv \
        # git
        git \
        git-lfs \
        # permissions
        sudo \
        gnupg \
        ca-certificates \
        # networking & remote
        curl \
        openssh-client \
        wget \
        xdg-utils \
        # tiiks
        p7zip-full \
        # terminal
        bash-completion \
        nano \
        zsh \
        && \
    # cleanup
    apt -qq clean && \
    apt -qq autoclean && \
    apt -qq remove && \
    apt -qq autoremove -y && \
    rm -rf /var/lib/apt/lists

# Install Qt for Windows (Cross-Compile)
# We use "Another Qt installer(aqt)" to get the official MinGW binaries 
RUN python3 -m pip install aqtinstall --break-system-packages
# Installed to: /opt/qt/6.8.0/mingw_64
ARG QT_VER=6.8.0
RUN aqt install-qt windows desktop ${QT_VER} win64_mingw -O /opt/qt --external 7z

# Install Qt for linux
RUN apt update && \
    apt -qq install -y  \
        qt6-base-dev \
    && \
    # cleanup
    apt -qq clean && \
    apt -qq autoclean && \
    apt -qq remove && \
    apt -qq autoremove -y && \
    rm -rf /var/lib/apt/lists

# Install Clang APT Packages
# Checkout the website: https://llvm.org/
# Checkout the website: https://apt.llvm.org/
# Checkout the website: https://apt.llvm.org/noble/pool/main/l/llvm-toolchain-20/
ENV CLANG_MAJOR_VERSION="20"
RUN \
    # Download and add the official LLVM GPG key
    wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add - && \
    # # Add the LLVM repository
    echo "deb http://apt.llvm.org/$(lsb_release -sc)/ llvm-toolchain-$(lsb_release -sc)-${CLANG_MAJOR_VERSION} main" > /etc/apt/sources.list.d/llvm-${CLANG_MAJOR_VERSION}.list && \
    # Install APT packages
    # - clang-XX: The C/C++ compiler
    # - lld-XX: The fast linker
    # - libc++-XX-dev: The C++ standard library headers
    # - libc++abi-XX-dev: The C++ ABI library headers
    apt update && apt install -y --no-install-recommends \
        clang-${CLANG_MAJOR_VERSION} \
        lld-${CLANG_MAJOR_VERSION} \
        libc++-${CLANG_MAJOR_VERSION}-dev \
        libc++abi-${CLANG_MAJOR_VERSION}-dev \
        clang-tools-${CLANG_MAJOR_VERSION} \
        clang-${CLANG_MAJOR_VERSION}-doc \
        libclang-common-${CLANG_MAJOR_VERSION}-dev \
        libclang-${CLANG_MAJOR_VERSION}-dev \
        clang-format-${CLANG_MAJOR_VERSION}  \
        libclang1-${CLANG_MAJOR_VERSION} \
        clangd-${CLANG_MAJOR_VERSION} \
        clang-tidy-${CLANG_MAJOR_VERSION} \
    && \
    # Create symlinks so you can just run `clang++`
    ln -s /usr/bin/clang-${CLANG_MAJOR_VERSION} /usr/bin/clang && \
    ln -s /usr/bin/clang++-${CLANG_MAJOR_VERSION} /usr/bin/clang++ && \
    # cleanup
    apt -qq clean && \
    apt -qq autoclean && \
    apt -qq remove && \
    apt -qq autoremove -y && \
    rm -rf /var/lib/apt/lists

# Install CMake
ENV CMAKE_VERSION 3.31.8
RUN \
    # Install CMake
    cd /tmp && \
    wget --no-check-certificate https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz && \
    tar -xzvf cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz && \
    mv cmake-${CMAKE_VERSION}-linux-x86_64 /opt/cmake && \
    rm cmake-${CMAKE_VERSION}-linux-x86_64.tar.gz && \
    # Install CMake Tools
    apt update && \
    apt -qq install -y --no-install-recommends \
        graphviz \
    && \
    # cleanup
    apt -qq clean && \
    apt -qq autoclean && \
    apt -qq remove && \
    apt -qq autoremove -y && \
    rm -rf /var/lib/apt/lists
ENV PATH="/opt/cmake/bin:${PATH}"

# Create User
RUN deluser --remove-home ubuntu && \
    useradd --create-home --user-group --groups sudo --uid 1000 developer && \
    chown -R developer:developer /home/developer && \
    printf "\developer ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
ENV USER=developer \
    PATH=/home/developer/.local/bin:$PATH
USER developer
WORKDIR /home/developer

# Install ZSH
RUN sudo apt update && sudo apt -qq install -y --no-install-recommends \
        zsh \
        && \
    sh -c "$(curl -fsSL https://raw.github.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" && \
    rm .zshrc && \
    touch .zshrc && \
    echo '\n# ZSH' >> .zshrc && \
    echo "export ZSH=\"/home/developer/.oh-my-zsh\"" >> .zshrc && \
    echo "export ZSH_THEME=\"eastwood\"" >> .zshrc && \
    echo "plugins=(git)" >> .zshrc && \
    echo "source /home/developer/.oh-my-zsh/oh-my-zsh.sh" >> .zshrc

ENTRYPOINT ["/usr/bin/zsh"]