INTRODUCTION
------------
This module enables connectivity between a drupal Islandora application and the Newberry T-PEN application.

Specifically it:
-  Allows Drupal to create and destroy sessions in T-PEN upon login
-  Allows creation and deletion of T-PEN projects by a user by delivering a SC-Manifest payload to T-PEN
-  Manages a list of T-PEN projects created by a user
-  Allows super users to manage T-PEN projects of other users


REQUIREMENTS
------------
Drupal 7.x

Newberry TPEN (available at https://sourceforge.net/projects/newberrytpen/)


INSTALLATION
------------
1.  Place the module into the custom module directory and enable it.

2.  Configuration can be changed in the admin/config/system/tpenconnector page. 
	The default tpenconnector variables will be set automatically upon installation by reading `includes/TPENSSOInfo.inc`.

    (**Note:** As `TPENSSOInfo.inc` doesn't exist by default in the git repository, you need to copy `includes/TPENSSOInfo.inc.sample` and supply the required values).

3.  Go to admin/people/permissions
	Check 'Administrator' in the 'Administer tpenconnector module' block to allow access to the configuration page.
	To assign an admin role for T-PEN app, check 'Administrator' and 'Paleography Administrator' in the 'T-PEN admin user' block.
	

 Copyright 2016 University of Toronto Libraries

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
