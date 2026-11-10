import type {SidebarsConfig} from '@docusaurus/plugin-content-docs';

// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

/**
 * The sidebar mirrors the "Table of Contents" in the repository README.md.
 * When adding a page to the guide, add it to both the README and this file.
 *
 * Doc ids are file paths relative to `guide/`, without the `.md` extension.
 */

type Item = Extract<SidebarsConfig[string], unknown[]>[number];

const doc = (id: string, label: string): Item => ({
  type: 'doc',
  id,
  label,
});

/** Top-level category with a generated index page listing its content. */
const category = (
  label: string,
  slug: string,
  description: string,
  items: Item[],
): Item => ({
  type: 'category',
  label,
  link: {type: 'generated-index', slug: `/category/${slug}`, description},
  items,
});

/** Nested group of pages inside a top-level category. Collapsed until opened (or a page inside it is active). */
const group = (label: string, items: Item[]): Item => ({
  type: 'category',
  label,
  collapsed: true,
  items,
});

const sidebars: SidebarsConfig = {
  guideSidebar: [
    category(
      'Strings, Streams & Files',
      'strings-streams-files',
      'Working with text, character encodings, formatted output, streams and files.',
      [
        group('Strings', [
          doc('strings/std_string', 'std::string'),
          doc('strings/c_style_strings', 'C Style Strings'),
          doc('strings/string_literals', 'String Literals'),
          doc('strings/character_encoding', 'Character Encoding'),
          doc('strings/string_view', 'std::string_view (C++17)'),
        ]),
        group('Streams', [
          doc('strings/io_streams', 'IO Streams'),
          doc('strings/string_streams', 'String Streams'),
          doc('strings/printf', 'std::printf (C++11)'),
          doc('strings/std_format', 'std::format (C++20)'),
          doc('strings/print', 'std::print (C++23)'),
        ]),
        group('Files', [
          doc('strings/file_io', 'File IO'),
          doc('strings/file_streams', 'File Streams'),
        ]),
      ],
    ),
    category(
      'Containers',
      'containers',
      'Sequence, associative and unordered containers of the standard library, and how to iterate over them.',
      [
        group('Sequence Containers', [
          doc('containers/sequence/c_style_arrays', 'C-Style Arrays'),
          doc('containers/sequence/loops', 'Loops'),
          doc('containers/sequence/std_array', 'std::array'),
          doc('containers/sequence/dynamic_allocation', 'Dynamic Allocation'),
          doc('containers/sequence/iterators', 'Iterators'),
          doc(
            'containers/sequence/standard_library_functions',
            'Standard Library Functions (STL)',
          ),
          doc('containers/sequence/std_list', 'std::list'),
          doc('containers/sequence/std_deque', 'std::deque'),
        ]),
        group('Associative Containers', [
          doc('containers/associative/std_set', 'std::set'),
          doc('containers/associative/std_map', 'std::map'),
          doc('containers/associative/std_multiset', 'std::multiset'),
          doc('containers/associative/std_multimap', 'std::multimap'),
        ]),
        group('Unordered Associative Containers', [
          doc('containers/unordered_associative/std_unordered_set', 'std::unordered_set'),
          doc('containers/unordered_associative/std_unordered_map', 'std::unordered_map'),
          doc(
            'containers/unordered_associative/std_unordered_multiset',
            'std::unordered_multiset',
          ),
          doc(
            'containers/unordered_associative/std_unordered_multimap',
            'std::unordered_multimap',
          ),
        ]),
        group('Other Important Containers', [
          doc('containers/std_span', 'std::span (C++20)'),
          doc('containers/container_adaptors', 'Container Adaptors'),
        ]),
      ],
    ),
    category(
      'Memory Management',
      'memory',
      'References, pointers, smart pointers and copying objects safely.',
      [
        doc('memory/references', 'References'),
        doc('memory/pointers', 'Pointers'),
        doc('memory/smart_pointers', 'Smart Pointers'),
        doc(
          'memory/smart_pointers_in_functions',
          'How To Use Smart Pointers With Functions',
        ),
        doc('memory/shallow_copy_vs_deep_copy', 'Shallow Copy vs Deep Copy'),
      ],
    ),
    category(
      'Functions',
      'functions',
      'Function pointers, lambdas and the standard library callable wrappers and concepts.',
      [
        doc('functions/function_pointers', 'Function Pointers'),
        doc('functions/lambdas', 'Lambdas'),
        doc('functions/std_function', 'std::function (C++11)'),
        doc('functions/std_move_only_function', 'std::move_only_function (C++23)'),
        doc(
          'functions/invoke_and_invocable',
          'std::invoke, invoke_result_t & invocable (C++17/20)',
        ),
        doc('functions/std_predicate', 'std::predicate (C++20)'),
      ],
    ),
    category(
      'Object Oriented Programming',
      'oop',
      'Classes, object lifetime, copy and move semantics, inheritance and virtual functions.',
      [
        group('General', [
          doc('oop/classes_vs_structs', 'Classes vs Structs'),
          doc('oop/constructors', 'Constructors'),
          doc('oop/destructors', 'Destructors'),
          doc('oop/copy_and_move', 'Copy and Move Semantics'),
          doc('oop/default_delete', 'Default and Delete'),
          doc('oop/rule_of_five', 'Rule of Five'),
          doc('oop/rule_of_zero', 'Rule of Zero'),
          doc('oop/static', 'Static'),
          doc('oop/operator_overloading', 'Operator Overloading'),
          doc('oop/friend', 'Friend'),
          doc('oop/nesting_classes', 'Nesting Classes'),
          doc('oop/object_relationships', 'Object Relationships'),
        ]),
        group('Inheritance', [
          doc('oop/inheritance/introduction', 'Introduction'),
          doc('oop/inheritance/order_of_construction', 'Order of Construction'),
          doc('oop/inheritance/access_specifiers', 'Access Specifiers'),
          doc(
            'oop/inheritance/functions_and_overriding_behavior',
            'Functions and Overriding Behavior',
          ),
          doc('oop/inheritance/hiding_functionality', 'Hiding Functionality'),
        ]),
        group('Virtual Functions', [
          doc('oop/virtual_functions/introduction', 'Introduction'),
          doc(
            'oop/virtual_functions/override_and_final_specifier',
            'Override and Final Specifier',
          ),
          doc('oop/virtual_functions/covariant_return_types', 'Covariant Return Types'),
          doc('oop/virtual_functions/destructors', 'Destructors'),
          doc('oop/virtual_functions/pure_virtual', 'Pure Virtual'),
          doc('oop/virtual_functions/multiple_inheritance', 'Multiple Inheritance'),
          doc('oop/virtual_functions/object_slicing', 'Object Slicing'),
        ]),
      ],
    ),
    category(
      'Compilation, CMake & Tooling',
      'tooling',
      'How C++ code is compiled and linked, building projects with CMake, and everyday tooling.',
      [
        group('Compilation Process', [
          doc('tools/compilation/compilation_process', 'Compilation Process'),
          doc('tools/compilation/compilers', 'Compiler Overview'),
          doc('tools/compilation/build_generators', 'Build Generators'),
          doc('tools/compilation/dependency_management', 'Dependency Management'),
          doc('tools/compilation/abi', 'ABI (Application Binary Interface)'),
        ]),
        group('CMake', [
          doc('tools/cmake/introduction', 'Introduction'),
          doc('tools/cmake/generators', 'Generators'),
          doc('tools/cmake/variables', 'Variables'),
          doc('tools/cmake/targets', 'Targets'),
          doc('tools/cmake/cmakepresets', 'Presets'),
          doc('tools/cmake/best_practices', 'Best Practices'),
        ]),
        group('Tooling', [
          doc('tools/clang/tooling', 'Common Tools'),
          doc('oop/h_and_cpp_files', 'H and CPP Files'),
        ]),
      ],
    ),
    category(
      'Typing, Casting & ConstExpr',
      'typing',
      'Type deduction, casting between types and compile-time evaluation.',
      [
        doc('typing/auto', 'Auto'),
        group('Casting', [
          doc('typing/casting/implicit_casting', 'Implicit Casting'),
          doc('typing/casting/explicit_casting', 'Explicit Casting'),
          doc('typing/casting/static_cast', 'static_cast'),
          doc('typing/casting/dynamic_cast', 'dynamic_cast'),
        ]),
        doc('typing/using_constexpr', 'Using constexpr'),
        doc('typing/using_constval', 'Using consteval'),
      ],
    ),
    category(
      'Concurrency',
      'concurrency',
      'Threads, asynchronous tasks and futures in C++.',
      [
        doc('concurrency/introduction', 'Introduction'),
        group('STL Threads', [
          doc('concurrency/stl_threads/thread', 'std::thread'),
          doc('concurrency/stl_threads/async_and_future', 'std::async and std::future'),
        ]),
        doc('concurrency/posix_vs_stl_threads', 'POSIX vs STL Threads'),
      ],
    ),
    category(
      'Design Patterns & Idioms',
      'design-patterns',
      'Common C++ idioms such as RAII and PIMPL.',
      [
        group('Idioms', [
          doc('design_patterns/raii', 'RAII'),
          doc('design_patterns/pimpl', 'PIMPL'),
        ]),
      ],
    ),
  ],
};

export default sidebars;
