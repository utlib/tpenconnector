<?php

class MasterTPENProjectContentType {

    public static function check_existing_project($object_pid) {

        global $user;

        $eq = new EntityFieldQuery();
        $eq->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'master_t_pen_projects')
            ->propertyCondition('status', 1)
            ->propertyCondition('uid', $user->uid)
            ->fieldCondition('field_pid', 'value', $object_pid, '=');

        $result = $eq->execute();

        if (count($result) > 0) {
            reset($result['node']);
            $node = node_load(key($result['node']));
            return $node->field_t_pen_uri['und'][0]['value'];
        }

        return false;
    }
    
    public static function add_master_tpen_project_node($object_pid, $uri) {
        global $user;
        $new_node = new stdClass();

        $object_from_pid = islandora_object_load($object_pid);

        // mandatory fields
        $new_node->uid = $user->uid; // set the user id of this node
        $new_node->type = 'master_t_pen_projects'; // set the content type of the node (must use the machine name)
        $new_node->status = 1; // status 1 means published, 0 means unpublished
        $new_node->title = $object_from_pid->label." (".$object_pid.")";
        $new_node->language = "English";

        $new_node->field_pid['und'][0]['value'] = $object_pid;
        $new_node->field_t_pen_uri['und'][0]['value'] = $uri;

        // save the node to Drupal
        node_save($new_node);

    }

    public static function remove_master_tpen_project_node($object_pid, $uri) {
        global $user;

        $eq = new EntityFieldQuery();
        $eq->entityCondition('entity_type', 'node')
            ->entityCondition('bundle', 'master_t_pen_projects')
            ->propertyCondition('status', 1)
            ->propertyCondition('uid', $user->uid)
            ->fieldCondition('field_pid', 'value', $object_pid, '=');

        $result = $eq->execute();

        $node = node_load(key($result['node']));

        $nid_to_delete = $node->nid;

        node_delete($nid_to_delete);

    }

}