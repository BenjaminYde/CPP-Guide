/**
 * Ejected from @docusaurus/theme-classic (DocCardList is "safe" to eject).
 *
 * Renders a category's contents like the sidebar tree instead of a grid of cards:
 * - Consecutive pages share one panel, each page is a row with a file icon.
 * - Each group (sub-category) is its own panel: a folder header, then its pages as rows.
 * Order follows the sidebar, so the page reads top to bottom.
 */
import type {ReactNode} from 'react';
import clsx from 'clsx';
import Link from '@docusaurus/Link';
import {
  useCurrentSidebarSiblings,
  filterDocCardListItems,
  findFirstSidebarItemLink,
} from '@docusaurus/plugin-content-docs/client';
import type {
  PropSidebarItem,
  PropSidebarItemCategory,
  PropSidebarItemLink,
} from '@docusaurus/plugin-content-docs';
import type {Props} from '@theme/DocCardList';

import styles from './styles.module.css';

type Block =
  | {kind: 'pages'; items: PropSidebarItemLink[]}
  | {kind: 'group'; item: PropSidebarItemCategory};

/** Groups consecutive pages together; every group (category) gets a block of its own. */
function toBlocks(items: PropSidebarItem[]): Block[] {
  const blocks: Block[] = [];
  for (const item of items) {
    if (item.type === 'category') {
      blocks.push({kind: 'group', item});
    } else if (item.type === 'link') {
      const last = blocks.at(-1);
      if (last?.kind === 'pages') {
        last.items.push(item);
      } else {
        blocks.push({kind: 'pages', items: [item]});
      }
    }
  }
  return blocks;
}

function countLabel(count: number): string {
  return `${count} ${count === 1 ? 'item' : 'items'}`;
}

/** One row: a page (file icon) or a nested group (folder icon + item count). */
function Entry({item}: {item: PropSidebarItem}): ReactNode {
  if (item.type === 'html') {
    return null;
  }
  const isGroup = item.type === 'category';
  const href = isGroup ? findFirstSidebarItemLink(item) : item.href;
  if (!href) {
    return null;
  }
  return (
    <li>
      <Link to={href} className={clsx(styles.entry, isGroup ? styles.folder : styles.file)}>
        <span className={styles.label}>{item.label}</span>
        {isGroup && <span className={styles.count}>{item.items.length}</span>}
      </Link>
    </li>
  );
}

function PagesPanel({items}: {items: PropSidebarItemLink[]}): ReactNode {
  return (
    <section className={styles.panel}>
      <ul className={styles.entries}>
        {items.map((item, index) => (
          <Entry key={index} item={item} />
        ))}
      </ul>
    </section>
  );
}

function GroupPanel({item}: {item: PropSidebarItemCategory}): ReactNode {
  const href = findFirstSidebarItemLink(item);
  return (
    <section className={styles.panel}>
      {href && (
        <Link to={href} className={styles.groupHeader}>
          <span className={styles.groupTitle}>{item.label}</span>
          <span className={styles.count}>{countLabel(item.items.length)}</span>
        </Link>
      )}
      <ul className={clsx(styles.entries, styles.nested)}>
        {item.items.map((child, index) => (
          <Entry key={index} item={child} />
        ))}
      </ul>
    </section>
  );
}

function DocCardListForCurrentSidebarCategory({className}: Props): ReactNode {
  const items = useCurrentSidebarSiblings();
  return <DocCardList items={items} className={className} />;
}

export default function DocCardList(props: Props): ReactNode {
  const {items, className} = props;
  if (!items) {
    return <DocCardListForCurrentSidebarCategory {...props} />;
  }
  const blocks = toBlocks(filterDocCardListItems(items));
  return (
    <div className={clsx(styles.list, className)}>
      {blocks.map((block, index) =>
        block.kind === 'group' ? (
          <GroupPanel key={index} item={block.item} />
        ) : (
          <PagesPanel key={index} items={block.items} />
        ),
      )}
    </div>
  );
}
