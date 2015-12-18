REQUIREMENTS
------------
Drupal 7.x


INSTALLATION
------------
1.  Place the module into custom module directory.

2.  Go to admin/build/modules. Enable the module.

3.  Go to admin/config/system/tpenconnector
    and configure the redirect urls.
    The default values will be set automatically upon installation.
    Details can be found in includes/TPENSSOInfo.inc.sample.

4.  Go to admin/people/permissions
    Check 'Administrator' in the 'Administer tpenconnector module' block to allow access to the configuration page.
    To assign an admin role for T-PEN app, check 'Administrator' and 'Paleography Administrator' in the 'T-PEN admin user' block.