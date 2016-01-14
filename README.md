REQUIREMENTS
------------
Drupal 7.x


INSTALLATION
------------
1.  Place the module into the custom module directory and enable it.

2.  Configuration can be changed in the admin/config/system/tpenconnector page. 
	The default tpenconnector variables will be set automatically upon installation. 
	TPENSSOInfo.inc (includes/TPENSSOInfo.inc) includes them. As TPENSSOInfo.inc doesn't exist in the git repository, you need to copy it from the dev or production site if you want to create your own local dev environment. 

3.  Go to admin/people/permissions
	Check 'Administrator' in the 'Administer tpenconnector module' block to allow access to the configuration page.
	To assign an admin role for T-PEN app, check 'Administrator' and 'Paleography Administrator' in the 'T-PEN admin user' block.