import {useEffect, useState, type ReactNode} from 'react';
import clsx from 'clsx';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';

import styles from './SupportLinks.module.css';

type SupportFields = {githubRepo: string; repoUrl: string; paypalUrl: string};

export function useSupportFields(): SupportFields {
  return useDocusaurusContext().siteConfig.customFields as SupportFields;
}

const CACHE_KEY = 'cpp-guide:github-stars';
const CACHE_TTL_MS = 60 * 60 * 1000; // the unauthenticated GitHub API allows 60 requests/hour per IP

/** Live star count from the GitHub API, cached per browser session. null until known (or on failure). */
function useGitHubStars(repo: string): number | null {
  const [stars, setStars] = useState<number | null>(null);

  useEffect(() => {
    try {
      const cached = JSON.parse(sessionStorage.getItem(CACHE_KEY) ?? 'null');
      if (cached && Date.now() - cached.time < CACHE_TTL_MS) {
        setStars(cached.stars);
        return;
      }
    } catch {
      // storage unavailable; fall through to fetch
    }

    let cancelled = false;
    fetch(`https://api.github.com/repos/${repo}`)
      .then((res) => (res.ok ? res.json() : Promise.reject(res.status)))
      .then((data: {stargazers_count: number}) => {
        if (cancelled) return;
        setStars(data.stargazers_count);
        try {
          sessionStorage.setItem(
            CACHE_KEY,
            JSON.stringify({stars: data.stargazers_count, time: Date.now()}),
          );
        } catch {
          // ignore
        }
      })
      .catch(() => {
        // rate-limited or offline: the button still works, just without a count
      });
    return () => {
      cancelled = true;
    };
  }, [repo]);

  return stars;
}

function formatCount(n: number): string {
  return n >= 1000 ? `${(n / 1000).toFixed(n >= 10000 ? 0 : 1).replace(/\.0$/, '')}k` : String(n);
}

function GitHubIcon(): ReactNode {
  return (
    <svg className={styles.icon} viewBox="0 0 16 16" aria-hidden="true">
      <path
        fill="currentColor"
        d="M8 0C3.58 0 0 3.58 0 8c0 3.54 2.29 6.53 5.47 7.59.4.07.55-.17.55-.38 0-.19-.01-.82-.01-1.49-2.01.37-2.53-.49-2.69-.94-.09-.23-.48-.94-.82-1.13-.28-.15-.68-.52-.01-.53.63-.01 1.08.58 1.23.82.72 1.21 1.87.87 2.33.66.07-.52.28-.87.51-1.07-1.78-.2-3.64-.89-3.64-3.95 0-.87.31-1.59.82-2.15-.08-.2-.36-1.02.08-2.12 0 0 .67-.21 2.2.82.64-.18 1.32-.27 2-.27.68 0 1.36.09 2 .27 1.53-1.04 2.2-.82 2.2-.82.44 1.1.16 1.92.08 2.12.51.56.82 1.27.82 2.15 0 3.07-1.87 3.75-3.65 3.95.29.25.54.73.54 1.48 0 1.07-.01 1.93-.01 2.2 0 .21.15.46.55.38A8.013 8.013 0 0016 8c0-4.42-3.58-8-8-8z"
      />
    </svg>
  );
}

function StarIcon(): ReactNode {
  return (
    <svg className={styles.icon} viewBox="0 0 16 16" aria-hidden="true">
      <path
        fill="currentColor"
        d="M8 .25a.75.75 0 01.67.42l1.88 3.81 4.2.61a.75.75 0 01.42 1.28l-3.04 2.96.72 4.19a.75.75 0 01-1.09.79L8 12.33l-3.76 1.98a.75.75 0 01-1.09-.79l.72-4.19L.83 6.37a.75.75 0 01.42-1.28l4.2-.61L7.33.67A.75.75 0 018 .25z"
      />
    </svg>
  );
}

function HeartIcon(): ReactNode {
  return (
    <svg className={styles.icon} viewBox="0 0 16 16" aria-hidden="true">
      <path
        fill="currentColor"
        d="M8 14.25l-.53-.47C3.3 10.1.75 7.8.75 5.1.75 2.9 2.48 1.25 4.6 1.25c1.2 0 2.36.56 3.1 1.44a4.1 4.1 0 013.1-1.44c2.12 0 3.85 1.65 3.85 3.85 0 2.7-2.55 5-6.72 8.68L8 14.25z"
      />
    </svg>
  );
}

type ButtonProps = {className?: string; size?: 'sm' | 'lg'};

/** "GitHub ★ Star | 123" button linking to the repo. */
export function GitHubStarButton({className, size = 'sm'}: ButtonProps): ReactNode {
  const {githubRepo, repoUrl} = useSupportFields();
  const stars = useGitHubStars(githubRepo);
  return (
    <a
      className={clsx(styles.button, styles.star, size === 'lg' && styles.lg, className)}
      href={repoUrl}
      target="_blank"
      rel="noopener noreferrer"
      aria-label={`Star ${githubRepo} on GitHub${stars !== null ? ` (${stars} stars)` : ''}`}>
      <span className={styles.label}>
        <GitHubIcon />
        <StarIcon />
        Star
      </span>
      {stars !== null && <span className={styles.count}>{formatCount(stars)}</span>}
    </a>
  );
}

/** "♥ Support" button linking to PayPal. */
export function SupportButton({className, size = 'sm'}: ButtonProps): ReactNode {
  const {paypalUrl} = useSupportFields();
  return (
    <a
      className={clsx(styles.button, styles.support, size === 'lg' && styles.lg, className)}
      href={paypalUrl}
      target="_blank"
      rel="noopener noreferrer"
      aria-label="Support this guide via PayPal">
      <span className={styles.label}>
        <HeartIcon />
        Support
      </span>
    </a>
  );
}

/* Navbar wrappers: in the mobile sidebar, render as a full-width menu row */
type NavbarItemProps = {mobile?: boolean};

export function NavbarGitHubStar({mobile}: NavbarItemProps): ReactNode {
  return (
    <div className={clsx(mobile ? 'menu__list-item' : 'navbar__item', styles.navItem)}>
      <GitHubStarButton className={mobile ? styles.mobile : undefined} />
    </div>
  );
}

export function NavbarSupport({mobile}: NavbarItemProps): ReactNode {
  return (
    <div className={clsx(mobile ? 'menu__list-item' : 'navbar__item', styles.navItem)}>
      <SupportButton className={mobile ? styles.mobile : undefined} />
    </div>
  );
}
