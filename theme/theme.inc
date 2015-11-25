<?php


function tpenconnector_preprocess_islandora_solr_with_tpen(&$variables) {
  $results = $variables['results'];
  global $user;
  $roles = $user->roles;

  foreach ($results as $key => $result) {
    $pid = $result['PID'];
    $ds = islandora_datastream_load("RELS-EXT", $pid);
    if (isset($ds->content)) {
      if (strpos($ds->content, 'info:fedora/paleography:manuscripts') && user_is_logged_in()  && in_array('administrator', $roles)) { 
        if (check_master_tpen__($pid)) {
            $master_tpen_title = "Edit Master T-PEN Project";
        } else {
            $master_tpen_title = "Create Master T-PEN Project";
        }

        $tpen_master_url = "islandora/object/".$pid."/transcribe_master";
        $variables['results'][$key]['master_tpen_btn'] = l($master_tpen_title, $tpen_master_url, array('html' => TRUE, 'attributes' => array('master_tpen_title' => $master_tpen_title, 'target' => '_blank', 'class' => 'tpen-link-base')));
      }

      if (strpos($ds->content, 'info:fedora/paleography:manuscripts') && user_is_logged_in() && check_master_tpen__($pid)) { 
        $transcribe_tpen_title = "Transcribe with T-PEN";
        $tpen_url = "islandora/object/".$pid."/transcribe";
        $variables['results'][$key]['transcribe_tpen_btn'] = l($transcribe_tpen_title, $tpen_url, array('html' => TRUE, 'attributes' => array('transcribe_tpen_title' => $transcribe_tpen_title, 'target' => '_blank', 'class' => 'tpen-link-base')));
      }

    }
  }
}

function check_master_tpen__($pid) {

    $query = new EntityFieldQuery();

    $query->entityCondition('entity_type', 'node')
        ->entityCondition('bundle','master_t_pen_projects')
        ->propertyCondition('status', 1)
        ->fieldCondition('field_pid', 'value', $pid, '=');


    $results = $query->execute();
    return count($results) > 0;
}

function tpenconnector_preprocess_islandora_basic_collection_grid_with_tpen(&$variables) {
  __process_display__($variables);
}

function tpenconnector_preprocess_islandora_basic_collection_with_tpen(&$variables) {
  __process_display__($variables);
}

function __process_display__(&$variables) {

  $results = &$variables['collection_results'];
  $islandora_object = &$variables['islandora_object'];

  $listings = $variables['associated_objects_array'];

  global $user;

  $roles = $user->roles;

  foreach ($listings as $key=>$value) {

    $pid = $value['pid'];

    $obj = $value['object'];

    $parent_obj = $obj->relationships->get('info:fedora/fedora-system:def/relations-external#', 'isMemberOfCollection');

    $parent_obj_pid = $parent_obj[0]['object']['value'];

    //if user is logged in && master project is already created for this particular pid, we show 'Transcribe with T-Pen button'
    if ($parent_obj_pid === 'paleography:manuscripts' && user_is_logged_in() && check_master_tpen__($pid)) {
      $transcribe_tpen_title = "Transcribe with T-PEN";

      $tpen_url = "islandora/object/".$pid."/transcribe";
      $variables['associated_objects_array'][$key]['transcribe_tpen_btn'] = l($transcribe_tpen_title, $tpen_url, array('html' => TRUE, 'attributes' => array('transcribe_tpen_title' => $transcribe_tpen_title, 'target' => '_blank', 'class' => 'tpen-link-base')));
    }

    if ($parent_obj_pid === 'paleography:manuscripts' && user_is_logged_in() && in_array('administrator', $roles)) {
      if (check_master_tpen__(($pid))) {
        $master_tpen_title = "Edit Master T-PEN project";
      } else {
        $master_tpen_title = "Create Master T-PEN project";
      }

      $tpen_master_url = "islandora/object/".$pid."/transcribe_master"; 
      $variables['associated_objects_array'][$key]['master_tpen_btn'] = l($master_tpen_title, $tpen_master_url, array('html' => TRUE, 'attributes' => array('master_tpen_title' => $master_tpen_title, 'target' => '_blank', 'class' => 'tpen-link-base')));
    }
  }
}