// Registers custom navbar item types used in docusaurus.config.ts (type: 'custom-*')
import ComponentTypes from '@theme-original/NavbarItem/ComponentTypes';
import {NavbarGitHubStar, NavbarSupport} from '@site/src/components/SupportLinks';

export default {
  ...ComponentTypes,
  'custom-githubStarButton': NavbarGitHubStar,
  'custom-supportButton': NavbarSupport,
};
