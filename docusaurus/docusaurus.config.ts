import {themes as prismThemes} from 'prism-react-renderer';
import type {Config} from '@docusaurus/types';
import type * as Preset from '@docusaurus/preset-classic';
import type {PluginOptions as SearchLocalOptions} from '@easyops-cn/docusaurus-search-local';
import remarkGithubAdmonitionsToDirectives from 'remark-github-admonitions-to-directives';
import remarkCodeLanguageAliases from './src/remark/codeLanguageAliases';

// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

const githubRepo = 'BenjaminYde/CPP-Guide';
const repoUrl = `https://github.com/${githubRepo}`;
const paypalUrl = 'https://paypal.me/BenjaminYde';

const config: Config = {
  title: 'C++ Guide',
  tagline:
    'A guide and quick reference for C++ developers: to the point, with code examples and best practices.',
  favicon: 'img/logo.svg',

  // Monospace font for code (used by --ifm-font-family-monospace in src/css/custom.css)
  headTags: [
    {tagName: 'link', attributes: {rel: 'preconnect', href: 'https://fonts.googleapis.com'}},
    {
      tagName: 'link',
      attributes: {rel: 'preconnect', href: 'https://fonts.gstatic.com', crossorigin: 'anonymous'},
    },
  ],
  stylesheets: [
    'https://fonts.googleapis.com/css2?family=JetBrains+Mono:ital,wght@0,100..800;1,100..800&display=swap',
  ],

  // Read by the GitHub star / support buttons (src/components/SupportLinks.tsx)
  customFields: {githubRepo, repoUrl, paypalUrl},

  // Future flags, see https://docusaurus.io/docs/api/docusaurus-config#future
  future: {
    v4: true, // Improve compatibility with the upcoming Docusaurus v4
  },

  // GitHub Pages deployment (https://docusaurus.io/docs/deployment#deploying-to-github-pages)
  url: 'https://benjaminyde.github.io',
  baseUrl: '/CPP-Guide/',
  organizationName: 'BenjaminYde',
  projectName: 'CPP-Guide',
  trailingSlash: false,

  // Fail the build on any broken link, anchor or image
  onBrokenLinks: 'throw',
  onBrokenAnchors: 'throw',

  markdown: {
    // .md files are parsed as CommonMark (same as GitHub), .mdx files as MDX
    format: 'detect',
    // Don't turn text like `std::thread::id` into emoji (`:thread:`)
    emoji: false,
    hooks: {
      onBrokenMarkdownLinks: 'throw',
      onBrokenMarkdownImages: 'throw',
    },
  },

  i18n: {
    defaultLocale: 'en',
    locales: ['en'],
  },

  presets: [
    [
      'classic',
      {
        docs: {
          // The guide content lives at the repository root so it stays
          // readable on GitHub; the website reads it from there.
          path: '../guide',
          routeBasePath: 'guide',
          sidebarPath: './sidebars.ts',
          editUrl: ({docPath}) => `${repoUrl}/edit/main/guide/${docPath}`,
          showLastUpdateTime: true,
          beforeDefaultRemarkPlugins: [
            // GitHub alerts (> [!NOTE]) -> Docusaurus admonitions (:::note)
            remarkGithubAdmonitionsToDirectives,
            // ```c++ -> ```cpp for Prism syntax highlighting
            remarkCodeLanguageAliases,
          ],
        },
        blog: false,
        theme: {
          customCss: './src/css/custom.css',
        },
      } satisfies Preset.Options,
    ],
  ],

  themes: [
    [
      // Offline search: the index is generated at build time and served as
      // static files, so it works on GitHub Pages without any external service.
      '@easyops-cn/docusaurus-search-local',
      {
        hashed: true,
        language: ['en'],
        indexDocs: true,
        indexBlog: false,
        indexPages: false,
        docsRouteBasePath: '/guide',
        docsDir: '../guide',
        // Keep words like "this", "new" and "do" searchable
        removeDefaultStopWordFilter: true,
        highlightSearchTermsOnTargetPage: true,
        explicitSearchResultPath: true,
      } satisfies SearchLocalOptions,
    ],
  ],

  themeConfig: {
    metadata: [
      {
        name: 'keywords',
        content: 'c++, cpp, modern c++, stl, cmake, guide, reference',
      },
    ],
    colorMode: {
      defaultMode: 'dark',
      respectPrefersColorScheme: false,
    },
    // "On this page" list: only top-level sections (## headings), no subsections
    tableOfContents: {
      minHeadingLevel: 2,
      maxHeadingLevel: 2,
    },
    docs: {
      sidebar: {
        hideable: true,
        autoCollapseCategories: true,
      },
    },
    navbar: {
      // Logo + "Home" link to the landing page; the docs link below carries the site name
      title: 'Home',
      logo: {
        alt: 'C++ Guide Logo',
        src: 'img/logo.svg',
      },
      items: [
        {
          type: 'docSidebar',
          sidebarId: 'guideSidebar',
          position: 'left',
          label: 'C++ Guide',
        },
        {
          type: 'search',
          position: 'right',
        },
        // Custom item types are registered in src/theme/NavbarItem/ComponentTypes.tsx
        {
          type: 'custom-supportButton',
          position: 'right',
        },
        {
          type: 'custom-githubStarButton',
          position: 'right',
        },
      ],
    },
    footer: {
      style: 'dark',
      links: [
        {
          title: 'Guide',
          items: [
            {label: 'Strings, Streams & Files', to: '/guide/category/strings-streams-files'},
            {label: 'Containers', to: '/guide/category/containers'},
            {label: 'Memory Management', to: '/guide/category/memory'},
            {label: 'Functions', to: '/guide/category/functions'},
            {label: 'Templates', to: '/guide/category/templates'},
            {label: 'Object Oriented Programming', to: '/guide/category/oop'},
            {label: 'Compilation, CMake & Tooling', to: '/guide/category/tooling'},
            {label: 'Typing, Casting & ConstExpr', to: '/guide/category/typing'},
            {label: 'Concurrency', to: '/guide/category/concurrency'},
            {label: 'Design Patterns & Idioms', to: '/guide/category/design-patterns'},
          ],
        },
        {
          title: 'References',
          items: [
            {label: 'cppreference', href: 'https://en.cppreference.com/w/'},
            {
              label: 'C++ Core Guidelines',
              href: 'https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines',
            },
            {label: 'LearnCpp', href: 'https://www.learncpp.com/'},
          ],
        },
        {
          title: 'More',
          items: [
            {label: '⭐ Star on GitHub', href: repoUrl},
            {label: '☕ Support via PayPal', href: paypalUrl},
            {label: 'Code Examples', href: `${repoUrl}/tree/main/code_examples`},
          ],
        },
      ],
      copyright: `Copyright © ${new Date().getFullYear()} Benjamin Yde. Built with Docusaurus.`,
    },
    prism: {
      theme: prismThemes.vsDark, // code blocks are dark in both themes (VS Code Dark+)
      darkTheme: prismThemes.vsDark, // VS Code Dark+
      // c, cpp, json, yaml and markdown are bundled by default
      additionalLanguages: ['bash', 'cmake'],
    },
  } satisfies Preset.ThemeConfig,
};

export default config;
