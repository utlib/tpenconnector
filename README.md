REQUIREMENTS
------------
Drupal 7.x


INSTALLATION
------------
1.  Place the module into the custom module directory.

2.  Enable it. 

3.  Configuration can be changed in the admin/config/system/tpenconnector page. The default variables will be set automatically upon installation. 
TPENSSOInfo.inc (includes/TPENSSOInfo.inc) does that work and it doesn't exit in the git repository. If you want to create your local dev environment, you need to copy the file from the dev site. 

4.  Go to admin/people/permissions
    Check 'Administrator' in the 'Administer tpenconnector module' block to allow access to the configuration page.
    To assign an admin role for T-PEN app, check 'Administrator' and 'Paleography Administrator' in the 'T-PEN admin user' block.