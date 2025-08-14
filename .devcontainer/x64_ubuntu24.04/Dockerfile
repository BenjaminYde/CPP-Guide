FROM ubuntu:24.04

# Development APT packages
RUN apt -qq update && \
    apt -qq install -y --no-install-recommends \
        apt-utils \
        autoconf \
        automake \
        bash-completion \    
        build-essential \
        ca-certificates \
        curl \
        dos2unix \
        g++ \
        gcc \
        gdb \
        git \
        locales-all \
        make \
        ninja-build \
        python-is-python3 \
        python3 \
        python3-dev \
        python3-pip \
        rsync \
        ssh \
        sudo \
        tar \
        unzip \
        wget \
        zip \
        && \
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

# Download and install Clang binaries
ENV CLANG_VERSION 20.1.8
RUN \
    # Download
    cd /tmp && \
    curl -L -O https://github.com/llvm/llvm-project/releases/download/llvmorg-${CLANG_VERSION}/LLVM-${CLANG_VERSION}-Linux-X64.tar.xz && \
    tar -xJvf LLVM-${CLANG_VERSION}-Linux-X64.tar.xz && \
    rm LLVM-${CLANG_VERSION}-Linux-X64.tar.xz && \
    # Install
    mkdir /opt/LLVM-${CLANG_VERSION} && \
    mv LLVM-${CLANG_VERSION}-Linux-X64/* /opt/LLVM-${CLANG_VERSION}
ENV PATH="/opt/LLVM-${CLANG_VERSION}/bin:${PATH}"

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