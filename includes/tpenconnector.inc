<?php
/**
 * @file tpenconnector.inc
 * This file is where non Drupal hooks code live
 */

function tpenconnector_get_manifest($object_pid = '') {
	global $user;
	
	// first, check if a project already exists in drupal for this user/ms 
	
	$uri = TPENProjectContentType::check_existing_project($object_pid);
	
	if (!$uri) {
		// copy the master project
		$master_uri = MasterTPENProjectContentType::check_existing_project($object_pid);
		
		if ($master_uri) {
			// the project id is the numerical suffix of the tpen uri
			preg_match('/[0-9]*$/', $master_uri, $project_id_matches);
			$project_id = $project_id_matches[0];
			
			$uri = TPEN::copy_project($project_id, $user->name);
			
			if ($uri === false) {
				drupal_set_message('Oops.  We were not able to create your project.  Please try again and if problems persist, please contact collections@library.utoronto.ca', 'error');
				header('Location: ' . $_SERVER['HTTP_REFERER']);
				exit;
			}

			// add uri to drupal for this user
			TPENProjectContentType::add_tpen_project_node($object_pid, $uri);
		}
	}
	
	preg_match('/[0-9]*$/', $uri, $project_id_matches);
	$project_id = $project_id_matches[0];
	
	// add /transcribe to URI for transcription interface URL
	header("Location: " . variable_get('tpenconnector_tpen_url') . "/project/" . $project_id . "/transcribe");
}


function tpenconnector_get_manifest_master($object_pid = '', $from_menutab = FALSE) {
	global $user;
	
	$object_from_pid = islandora_object_load($object_pid);
	$title           = $object_from_pid->label;
	if (strlen($title) >= 500) {
		$title = substr($title, 0, 500) . "...";
	}
	
	// first, check if a project already exists in drupal for this user/ms 
	$uri = MasterTPENProjectContentType::check_existing_project($object_pid);
	
	if (!$from_menutab) {
		if (!$uri) {
			$manifest = islandora_datastream_load("SC", $object_pid);
			$uri      = TPEN::create_project($manifest, urlencode($title), $user->name);
			//$uri = "http://tpendev:8080/tpen/project/4363";
			
			// add uri to drupal for this user
			MasterTPENProjectContentType::add_master_tpen_project_node($object_pid, $uri);
			drupal_set_message(t('Successfully created Master T-PEN Project for ' . $title . ' (' . $object_pid . ')'), 'status');
			echo "success";
		} else {
			
			preg_match('/[0-9]*$/', $uri, $project_id_matches);
			$project_id = $project_id_matches[0];
			// add /transcribe to URI for transcription interface URL
			header("Location: " . variable_get('tpenconnector_tpen_url') . "/project/" . $project_id . "/transcribe");
		}
	} else {
		//it's being called from the menu tab at book page
		
		if (!$uri) {
			$manifest = islandora_datastream_load("SC", $object_pid);
			$uri      = TPEN::create_project($manifest, urlencode($title), $user->name);
			MasterTPENProjectContentType::add_master_tpen_project_node($object_pid, $uri);
		}
		
		preg_match('/[0-9]*$/', $uri, $project_id_matches);
		$project_id = $project_id_matches[0];
		$url        = variable_get('tpenconnector_tpen_url') . "/project/" . $project_id . "/transcribe";
		drupal_goto($url, array(
			'external' => TRUE
		));
	}
	
}

function tpenconnector_delete($form, &$form_state, $prefix) {
	global $user;
	$eq = new EntityFieldQuery();
	$eq->entityCondition('entity_type', 'node')->entityCondition('bundle', 't_pen_projects')->propertyCondition('status', 1)->propertyCondition('uid', $user->uid)->fieldCondition('field_pid', 'value', $prefix, '=');
	
	$result = $eq->execute();
	
	$node = node_load(key($result['node']));
	
	$form['title']      = array(
		'#markup' => 'Are you sure you want to delete <em><strong>' . $node->title . '</strong></em>?'
	);
	$form['object_pid'] = array(
		'#value' => $node->field_pid['und'][0]['value'],
		'#type' => 'hidden'
	);
	$form['actions']    = array(
		'#type' => 'actions'
	);
	
	$form['actions']['update'] = array(
		'#type' => 'submit',
		'#value' => t('Delete this T-PEN Project'),
		'#weight' => 0,
		'#submit' => array(
			'_delete_project'
		)
	);
	
	return $form;
}

function tpenconnector_master_delete($form, &$form_state, $prefix) {
	global $user;
	$eq = new EntityFieldQuery();
	$eq->entityCondition('entity_type', 'node')->entityCondition('bundle', 'master_t_pen_projects')->propertyCondition('status', 1)->fieldCondition('field_pid', 'value', $prefix, '=');
	
	$result = $eq->execute();
	
	$node = node_load(key($result['node']));
	
	$form['title']      = array(
		'#markup' => 'Are you sure you want to delete <em><strong>' . $node->title . '</strong></em>?'
	);
	$form['object_pid'] = array(
		'#value' => $node->field_pid['und'][0]['value'],
		'#type' => 'hidden'
	);
	$form['actions']    = array(
		'#type' => 'actions'
	);
	
	$form['actions']['update'] = array(
		'#type' => 'submit',
		'#value' => t('Delete this Master T-PEN Project'),
		'#weight' => 0,
		'#submit' => array(
			'_delete_project_master'
		)
	);
	
	return $form;
}

function _delete_project($form, &$form_state) {
	global $user;
	$object_pid = $form_state['values']['object_pid'];
	
	// first, check if a project already exists in drupal for this user/ms 
	$uri = TPENProjectContentType::check_existing_project($object_pid);
	if ($uri) {
		// the project id is the numerical suffix of the tpen uri
		preg_match('/[0-9]*$/', $uri, $project_id_matches);
		$project_id = $project_id_matches[0];
		// delete project in TPEN first, and only proceed if successful
		if (TPEN::delete_project($project_id, $user->name) !== false) {
			TPENProjectContentType::remove_tpen_project_node($object_pid, $uri);
			$form_state['redirect'] = '/my-transcriptions';
			drupal_set_message(t('T-PEN project removed successfully'));
		} else {
			$form_state['redirect'] = '/my-transcriptions';
			drupal_set_message(t('Unable to remove project from T-PEN app.'), 'error');
		}
	}
}

function _delete_project_master($form, &$form_state) {
	global $user;
	$object_pid = $form_state['values']['object_pid'];
	
	// first, check if a project already exists in drupal for this user/ms 
	$uri = MasterTPENProjectContentType::check_existing_project($object_pid);
	
	if ($uri) {
		// the project id is the numerical suffix of the tpen uri
		preg_match('/[0-9]*$/', $uri, $project_id_matches);
		$project_id = $project_id_matches[0];
		// delete project in TPEN first, and only proceed if successful
		if (TPEN::delete_project($project_id, $user->name) !== false) {
			MasterTPENProjectContentType::remove_master_tpen_project_node($object_pid, $uri);
			$form_state['redirect'] = '/master-transcriptions';
			drupal_set_message(t('Master T-PEN project removed successfully'));
		} else {
			$form_state['redirect'] = '/master-transcriptions';
			drupal_set_message(t('Unable to remove project from T-PEN app.'), 'error');
		}
	}
}


function tpenconnector_share($form, &$form_state, $prefix) {
	global $user;
	$eq = new EntityFieldQuery();
	$eq->entityCondition('entity_type', 'node')->entityCondition('bundle', 't_pen_projects')->propertyCondition('status', 1)->propertyCondition('uid', $user->uid)->fieldCondition('field_pid', 'value', $prefix, '=');
	
	$result = $eq->execute();
	
	$node = node_load(key($result['node']));
	
	$object_pid = $node->field_pid['und'][0]['value'];
	
	// first, check if a project already exists in drupal for this user/ms
	$uri = TPENProjectContentType::check_existing_project($object_pid);
	
	if ($uri) {
		// the project id is the numerical suffix of the tpen uri
		preg_match('/[0-9]*$/', $uri, $project_id_matches);
		$project_id = $project_id_matches[0];
		
		if (!empty($project_id)) {
			$url = variable_get('tpenconnector_tpen_url') . "/project.html?projectID=" . $project_id;
			drupal_goto($url, array(
				'external' => TRUE
			));
		} else {
			$form_state['redirect'] = '/my-transcriptions';
			drupal_set_message(t('Unable to share project from T-PEN app.'), 'error');
		}
		
	}
}

function tpenconnector_edit_shared_project($project_id) {
	$url = variable_get('tpenconnector_tpen_url') . "/newberryTrans.html?projectID=" . $project_id;
	drupal_goto($url, array(
		'external' => TRUE
	));	
}
