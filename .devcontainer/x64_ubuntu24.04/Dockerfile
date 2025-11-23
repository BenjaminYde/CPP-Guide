FROM ubuntu:24.04

# Development APT packages
RUN apt -qq update && \
    apt -qq install -y --no-install-recommends \
        # general
        apt-utils \
        lsb-release \
        # general
        apt-utils \
        lsb-release \
        # building
        build-essential \
        software-properties-common \
        ninja-build \
        # git
        git \
        git-lfs \
        # permissions
        sudo \
        gnupg \
        ca-certificates \
        # networking & remote
        curl \
        wget \
        ssh \
        openssh-client \
        wget \
        xdg-utils \
        # terminal
        bash-completion \
        nano \
        locales-all \
        zsh \
        && \
    # cleanup
    apt -qq clean && \
    apt -qq autoclean && \
    apt -qq remove && \
    apt -qq autoremove

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
    
# Download and install Clang binaries
#ENV CLANG_VERSION 20.1.8
#RUN \
#    # Download
#    cd /tmp && \
#    curl -L -O https://github.com/llvm/llvm-project/releases/download/llvmorg-${CLANG_VERSION}/LLVM-${CLANG_VERSION}-Linux-X64.tar.xz && \
#    tar -xJvf LLVM-${CLANG_VERSION}-Linux-X64.tar.xz && \
#    rm LLVM-${CLANG_VERSION}-Linux-X64.tar.xz && \
#    # Install
#    mkdir /opt/LLVM-${CLANG_VERSION} && \
#    mv LLVM-${CLANG_VERSION}-Linux-X64/* /opt/LLVM-${CLANG_VERSION}
#ENV PATH="/opt/LLVM-${CLANG_VERSION}/bin:${PATH}"

# Install VCPKG
#RUN apt update && \
#    cd /tmp && \
#    wget --no-check-certificate https://github.com/microsoft/vcpkg/archive/refs/tags/2023.02.24.tar.gz && \
#    tar -xzvf 2023.02.24.tar.gz && \
#    mv vcpkg-2023.02.24 /opt/vcpkg && \
#    rm 2023.02.24.tar.gz && \
#    cd /opt/vcpkg && \
#    ./bootstrap-vcpkg.sh
#ENV PATH="/opt/vcpkg:${PATH}"

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