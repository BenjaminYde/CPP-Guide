import type {Nodes, Root} from 'mdast';

export type CodeLanguageAliasesOptions = {
  /** Map of code fence language (lowercase) -> Prism language name. */
  aliases?: Record<string, string>;
};

// GitHub accepts ```c++ but Prism only knows the language as "cpp".
const DEFAULT_ALIASES: Record<string, string> = {
  'c++': 'cpp',
};

/**
 * Remark plugin that rewrites code fence languages to the name Prism uses,
 * so the guide can keep GitHub-style fences (```c++) and still get syntax
 * highlighting on the website.
 */
export default function remarkCodeLanguageAliases(
  options: CodeLanguageAliasesOptions = {},
) {
  const aliases = {...DEFAULT_ALIASES, ...options.aliases};

  const visit = (node: Nodes): void => {
    if (node.type === 'code' && node.lang) {
      node.lang = aliases[node.lang.toLowerCase()] ?? node.lang;
    }
    if ('children' in node) {
      node.children.forEach(visit);
    }
  };

  return (tree: Root): void => visit(tree);
}
