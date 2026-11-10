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
        # networking & remote
        curl \
        openssh-client \
        wget \
        xdg-utils \
        # permissions
        sudo \
        gnupg \
        ca-certificates \
        # terminal
        bash-completion \
        nano \
        zsh

# ---------------------------------------------------------------------------
# Install Node.js - JavaScript runtime for Docusaurus (docs website)
# ---------------------------------------------------------------------------
# Checkout the website: https://nodejs.org/
# Checkout the website: https://github.com/nodesource/distributions
ENV NODE_MAJOR_VERSION=24
RUN --mount=type=cache,target=/var/cache/apt,sharing=locked \
    --mount=type=cache,target=/var/lib/apt,sharing=locked \
    # Add the NodeSource APT repository
    curl -fsSL https://deb.nodesource.com/setup_${NODE_MAJOR_VERSION}.x | bash - && \
    # Install Node.js (includes npm)
    apt -qq install -y --no-install-recommends \
        nodejs

# ---------------------------------------------------------------------------
# Setup User
# ---------------------------------------------------------------------------
RUN deluser --remove-home ubuntu && \
    useradd --create-home --user-group --groups sudo --uid 1000 developer && \
    chown -R developer:developer /home/developer && \
    printf "\ndeveloper ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
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