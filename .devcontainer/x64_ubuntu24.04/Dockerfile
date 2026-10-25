FROM ubuntu:24.04

# ---------------------------------------------------------------------------
# Install APT packages
# ---------------------------------------------------------------------------
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt,sharing=locked \
    apt -qq update && \
    apt -qq install -y --no-install-recommends \
        # general
        apt-utils \
        lsb-release \
        # git
        git \
        git-lfs \
        # files & io
        parallel \
        rsync \
        unzip \
        tree \
        # networking & remote
        curl \
        openssh-client \
        ssh \
        wget \
        xdg-utils \
        # cpp
        build-essential \
        ccache \
        autoconf \
        automake \
        dos2unix \
        gcc-14 \
        g++-14 \
        libstdc++-14-dev \
        gdb \
        make \
        ninja-build \
        software-properties-common \
        # permissions
        sudo \
        gnupg \
        ca-certificates \
        # terminal
        bash-completion \
        nano \
        zsh

# ---------------------------------------------------------------------------
# Configure GCC & G++ Compilers
# ---------------------------------------------------------------------------
RUN \ 
    # Register both versions
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 13 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-14 14 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 13 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-14 14 && \
    # Set 14 as default
    update-alternatives --set gcc /usr/bin/gcc-14 && \
    update-alternatives --set g++ /usr/bin/g++-14

# ---------------------------------------------------------------------------
# Install CMake - cross-platform build system generator
# ---------------------------------------------------------------------------
ENV CMAKE_VERSION=4.4.4
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
        graphviz
ENV PATH="/opt/cmake/bin:${PATH}"

# ---------------------------------------------------------------------------
# Install Clang - C/C++ compiler, linker and tools (clangd, clang-format, clang-tidy)
# ---------------------------------------------------------------------------
# Install Clang APT Packages
# Checkout the website: https://llvm.org/
# Checkout the website: https://apt.llvm.org/
# Checkout the website: https://apt.llvm.org/noble/pool/main/l/llvm-toolchain-22/
ENV CLANG_MAJOR_VERSION=22
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
        lldb-${CLANG_MAJOR_VERSION} \
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
    ln -s /usr/bin/clangd-${CLANG_MAJOR_VERSION} /usr/bin/clangd && \
    ln -s /usr/bin/clang-format-${CLANG_MAJOR_VERSION} /usr/bin/clang-format && \
    ln -s /usr/bin/clang-tidy-${CLANG_MAJOR_VERSION} /usr/bin/clang-tidy && \
    ln -s /usr/bin/lldb-${CLANG_MAJOR_VERSION} /usr/bin/lldb && \
    ln -s /usr/bin/lldb-dap-${CLANG_MAJOR_VERSION} /usr/bin/lldb-dap

# ---------------------------------------------------------------------------
# Setup User
# ---------------------------------------------------------------------------
RUN deluser --remove-home ubuntu && \
    useradd --create-home --user-group --groups sudo --uid 1000 developer && \
    chown -R developer:developer /home/developer && \
    printf "\developer ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
ENV USER=developer \
    PATH=/home/developer/.local/bin:$PATH
USER developer
WORKDIR /home/developer

# ---------------------------------------------------------------------------
# ZSH + Oh My Zsh
# ---------------------------------------------------------------------------
RUN sudo apt install -y zsh && \
    sudo chsh -s /usr/bin/zsh && \
    # Install oh my zsh
    sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" && \
    # configure .zshrc
    cd ~ && \
    touch .zshrc && \
    sed -i 's/ZSH_THEME=.*/ZSH_THEME="eastwood"/' .zshrc && \
    echo "ZSH Installed"

ENTRYPOINT ["/usr/bin/zsh"]