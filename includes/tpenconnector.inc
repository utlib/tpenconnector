<?php
/**
 * @file tpenconnector.inc
 * This file is where non Drupal hooks code live
 */

function tpenconnector_get_manifest($object_pid = '') {
    
    $manifest = islandora_datastream_load("SC", $object_pid);
    
    //drupal_add_http_header('Content-Type', 'application/json; utf-8');
    //echo $manifest->content;
    //echo rawurlencode($manifest->content);

    $session_id = TPEN::login();

    if ($session_id) {
        // first, check if a project already exists in drupal for this user/ms 

        $uri = TPENProjectContentType::check_existing_project($object_pid);
        if (!$uri) {
            // copy the master project
            $master_uri = MasterTPENProjectContentType::check_existing_project($object_pid);

            if ($master_uri) {
                 // the project id is the numerical suffix of the tpen uri
                 preg_match('/[0-9]*$/', $master_uri, $project_id_matches);
                 $project_id = $project_id_matches[0];
             
                 $uri = TPEN::copy_project($project_id, $session_id);
             
                 //$uri = "http://tpendev:8080/tpen/project/4363";
             
                 // add uri to drupal for this user
                 TPENProjectContentType::add_tpen_project_node($object_pid, $uri);
            }
        }

        // add /transcribe to URI for transcription interface URL
        header("Location: " . $uri . "/transcribe?session_id=" . $session_id);
    }else {
        echo "Error logging into T-PEN!";
    }
}

function tpenconnector_get_manifest_master($object_pid = '', $from_menutab = FALSE) {

    $manifest = islandora_datastream_load("SC", $object_pid);
    
    //drupal_add_http_header('Content-Type', 'application/json; utf-8');
    //echo $manifest->content;
    //echo rawurlencode($manifest->content);

    $session_id = TPEN::login();

    if ($session_id) {
        // first, check if a project already exists in drupal for this user/ms 
        $uri = MasterTPENProjectContentType::check_existing_project($object_pid);
        if (!$from_menutab) {
          if (!$uri) {
              $uri = TPEN::create_project($manifest);
              //$uri = "http://tpendev:8080/tpen/project/4363";

              // add uri to drupal for this user
              MasterTPENProjectContentType::add_master_tpen_project_node($object_pid, $uri);
              drupal_set_message(t('Successfully created Master T-PEN Project for '.$object_pid), 'status');
              echo "success";
          } else {

            // add /transcribe to URI for transcription interface URL
            header("Location: " . $uri . "/transcribe?session_id=" . $session_id);
          }
        } else {
          //it's being called from the menu tab at book page

          if (!$uri) {
            $uri = TPEN::create_project($manifest);
            MasterTPENProjectContentType::add_master_tpen_project_node($object_pid, $uri);
          }
          $url = $uri . "/transcribe?session_id=" . $session_id;
          drupal_goto($url, array('external' => TRUE));
        }
    }else {
        echo "Error logging into T-PEN!";
    }
   
   
   /*
   $ch = curl_init();
   curl_setopt($ch, CURLOPT_URL, TPEN_URL . "/createProject");
   curl_setopt($ch, CURLOPT_POST, 1);
   curl_setopt($ch, CURLOPT_POSTFIELDS, $manifest->content);
   curl_exec($ch);
   curl_close($ch);   
   */
}


function tpenconnector_delete($form, &$form_state, $prefix) {
    global $user;
    $eq = new EntityFieldQuery();
    $eq->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 't_pen_projects')
        ->propertyCondition('status', 1)
        ->propertyCondition('uid', $user->uid)
        ->fieldCondition('field_pid', 'value', $prefix, '=');

    $result = $eq->execute();

    $node = node_load(key($result['node']));

    $form['title'] = array(
      '#markup' => 'Are you sure you want to delete <em><strong>'.$node->title.'</strong></em>?'
    );
    $form['object_pid'] = array(
      '#value' =>  $node->field_pid['und'][0]['value'],
      '#type' => 'hidden'
    );
    $form['actions'] = array(
      '#type' => 'actions'
    );

    $form['actions']['update'] = array(
        '#type' => 'submit',
        '#value' => t('Delete this T-PEN Project'),
        '#weight' => 0,
        '#submit' => array('_delete_project')
    );

    return $form;
}

function tpenconnector_master_delete($form, &$form_state, $prefix) {
    global $user;
    $eq = new EntityFieldQuery();
    $eq->entityCondition('entity_type', 'node')
        ->entityCondition('bundle', 'master_t_pen_projects')
        ->propertyCondition('status', 1)
        ->fieldCondition('field_pid', 'value', $prefix, '=');

    $result = $eq->execute();

    $node = node_load(key($result['node']));

    $form['title'] = array(
      '#markup' => 'Are you sure you want to delete <em><strong>'.$node->title.'</strong></em>?'
    );
    $form['object_pid'] = array(
      '#value' =>  $node->field_pid['und'][0]['value'],
      '#type' => 'hidden'
    );
    $form['actions'] = array(
      '#type' => 'actions'
    );

    $form['actions']['update'] = array(
        '#type' => 'submit',
        '#value' => t('Delete this Master T-PEN Project'),
        '#weight' => 0,
        '#submit' => array('_delete_project_master')
    );

    return $form;
}

function _delete_project($form, &$form_state) {
    $object_pid = $form_state['values']['object_pid'];

    $session_id = TPEN::login();

    if ($session_id) {
        // first, check if a project already exists in drupal for this user/ms 
        $uri = TPENProjectContentType::check_existing_project($object_pid);
        if ($uri) {
           // the project id is the numerical suffix of the tpen uri
           preg_match('/[0-9]*$/', $uri, $project_id_matches);
           $project_id = $project_id_matches[0];
           // delete project in TPEN first, and only proceed if successful
           if (TPEN::delete_project($project_id, $session_id) !== false) {
              TPENProjectContentType::remove_tpen_project_node($object_pid, $uri);
              $form_state['redirect'] = '/tpen-transciption-project';
              drupal_set_message(t('T-PEN project removed successfully'));
           }else {
              $form_state['redirect'] = '/tpen-transciption-project';
              drupal_set_message(t('Unable to remove project from T-PEN app.'), 'error');
           }
        }
    }else {
        echo "Error logging into T-PEN!";
    }
}

function _delete_project_master($form, &$form_state) {
    $object_pid = $form_state['values']['object_pid'];

    $session_id = TPEN::login();

    if ($session_id) {
        // first, check if a project already exists in drupal for this user/ms 
        $uri = MasterTPENProjectContentType::check_existing_project($object_pid);

        if ($uri) {
           // the project id is the numerical suffix of the tpen uri
           preg_match('/[0-9]*$/', $uri, $project_id_matches);
           $project_id = $project_id_matches[0];
           // delete project in TPEN first, and only proceed if successful
           if (TPEN::delete_project($project_id, $session_id) !== false) {
              MasterTPENProjectContentType::remove_master_tpen_project_node($object_pid, $uri);
              $form_state['redirect'] = '/master-tpen-transciption-project';
              drupal_set_message(t('Master T-PEN project removed successfully'));
           }else {
              $form_state['redirect'] = '/master-tpen-transciption-project';
              drupal_set_message(t('Unable to remove project from T-PEN app.'), 'error');
           }
        }
    }else {
        echo "Error logging into T-PEN!";
    }
}




