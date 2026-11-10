import type {ReactNode} from 'react';
import clsx from 'clsx';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';
import Heading from '@theme/Heading';

import {GitHubStarButton, SupportButton, useSupportFields} from '@site/src/components/SupportLinks';

import styles from './index.module.css';

type Topic = {
  emoji: string;
  title: string;
  slug: string;
  description: string;
};

// Mirrors the categories in sidebars.ts (and the README table of contents)
const TOPICS: Topic[] = [
  {
    emoji: '📜',
    title: 'Strings, Streams & Files',
    slug: 'strings-streams-files',
    description: 'std::string, string_view, encodings, std::format, std::print, streams and file IO.',
  },
  {
    emoji: '📦',
    title: 'Containers',
    slug: 'containers',
    description: 'Arrays, iterators, sequence, associative and unordered containers, std::span.',
  },
  {
    emoji: '🧠',
    title: 'Memory Management',
    slug: 'memory',
    description: 'References, pointers, smart pointers and shallow vs deep copies.',
  },
  {
    emoji: '🎭',
    title: 'Functions',
    slug: 'functions',
    description: 'Function pointers, lambdas, std::function, std::invoke and callable concepts.',
  },
  {
    emoji: '🏛️',
    title: 'Object Oriented Programming',
    slug: 'oop',
    description: 'Constructors, copy and move semantics, rule of five, inheritance and virtual functions.',
  },
  {
    emoji: '🛠️',
    title: 'Compilation, CMake & Tooling',
    slug: 'tooling',
    description: 'The compilation process, compilers, ABI, CMake targets and presets, tooling.',
  },
  {
    emoji: '🔗',
    title: 'Typing, Casting & ConstExpr',
    slug: 'typing',
    description: 'auto, implicit and explicit casts, static_cast, dynamic_cast and constexpr.',
  },
  {
    emoji: '⚡',
    title: 'Concurrency',
    slug: 'concurrency',
    description: 'std::thread, std::async and std::future, POSIX vs STL threads.',
  },
  {
    emoji: '🎨',
    title: 'Design Patterns & Idioms',
    slug: 'design-patterns',
    description: 'Idioms like RAII and PIMPL.',
  },
];

function HomepageHeader(): ReactNode {
  const {siteConfig} = useDocusaurusContext();
  return (
    <header className={clsx('hero hero--primary', styles.heroBanner)}>
      <div className="container">
        <Heading as="h1" className="hero__title">
          {siteConfig.title}
        </Heading>
        <p className="hero__subtitle">{siteConfig.tagline}</p>
        <div className={styles.buttons}>
          <Link
            className="button button--primary button--lg"
            to="/guide/category/strings-streams-files">
            Start reading
          </Link>
          <GitHubStarButton size="lg" />
        </div>
      </div>
    </header>
  );
}

function TopicCard({emoji, title, slug, description}: Topic): ReactNode {
  return (
    <div className="col col--4 margin-bottom--lg">
      <Link to={`/guide/category/${slug}`} className={clsx('card', styles.topicCard)}>
        <div className="card__header">
          <Heading as="h3">
            <span className={styles.topicEmoji} aria-hidden="true">
              {emoji}
            </span>
            {title}
          </Heading>
        </div>
        <div className="card__body">
          <p>{description}</p>
        </div>
      </Link>
    </div>
  );
}

// Toolchain versions mirror .devcontainer/x64_ubuntu24.04/Dockerfile
const TOOLCHAIN = ['GCC 14', 'Clang 22', 'CMake 4.4', 'clangd', 'LLDB', 'clang-format'];

function DevContainerSection(): ReactNode {
  const {repoUrl} = useSupportFields();
  return (
    <section className={styles.devcontainer}>
      <div className={styles.devcontainerText}>
        <Heading as="h2">Run every example in a DevContainer</Heading>
        <p>
          The guide comes with runnable C++ examples in <code>code_examples</code>. The repository
          ships a Docker <strong>DevContainer</strong> for Visual Studio Code, so you get the same
          ready-to-use toolchain on Windows, macOS and Linux without installing compilers yourself.
        </p>
        <ul className={styles.toolchain} aria-label="Included tools">
          {TOOLCHAIN.map((tool) => (
            <li key={tool}>{tool}</li>
          ))}
        </ul>
        <div className={styles.devcontainerActions}>
          <Link className="button button--primary" href={`${repoUrl}/tree/main/code_examples`}>
            Browse the code examples
          </Link>
        </div>
      </div>
      <ol className={styles.steps}>
        <li>
          <span><strong>Install</strong> Docker and the VS Code <em>Dev Containers</em> extension.</span>
        </li>
        <li>
          <span><strong>Clone</strong> the repository and open the folder in VS Code.</span>
        </li>
        <li>
          <span><strong>Run</strong> <code>Dev Containers: Reopen in Container</code> and pick{' '}
          <code>cpp_guide</code>.</span>
        </li>
        <li>
          <span><strong>Build</strong> any example with CMake Tools, then run or debug it.</span>
        </li>
      </ol>
    </section>
  );
}

export default function Home(): ReactNode {
  const {siteConfig} = useDocusaurusContext();
  return (
    <Layout title="Home" description={siteConfig.tagline}>
      <HomepageHeader />
      <main className="container margin-vert--xl">
        <section className={styles.note}>
          <Heading as="h2">A note on reading this guide</Heading>
          <p>
            This guide is <strong>structured by topic, not by difficulty</strong>. There is no
            prescribed beginner-to-advanced path: jump straight to the section you need, whether
            that is <code>std::vector</code>, RAII or CMake Presets.
          </p>
        </section>
        <section>
          <Heading as="h2" className="margin-bottom--lg">
            Topics
          </Heading>
          <div className="row">
            {TOPICS.map((topic) => (
              <TopicCard key={topic.slug} {...topic} />
            ))}
          </div>
        </section>
        <DevContainerSection />
        <section className={styles.support}>
          <Heading as="h2">Enjoying the guide?</Heading>
          <p>
            This guide is free and open source. A GitHub star helps other developers find it, and
            if it saved you time, you can support its development via PayPal.
          </p>
          <div className={styles.buttons}>
            <GitHubStarButton size="lg" />
            <SupportButton size="lg" />
          </div>
        </section>
      </main>
    </Layout>
  );
}
