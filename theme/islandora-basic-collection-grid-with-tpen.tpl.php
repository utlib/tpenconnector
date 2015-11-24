<?php

/**
 * @file
 * islandora-basic-collection.tpl.php
 *
 * @TODO: needs documentation about file and variables
 */
?>

<div class="islandora islandora-basic-collection">
  <div class="islandora-basic-collection-grid clearfix">
  <?php foreach($associated_objects_array as $key => $value): ?>
    <dl class="islandora-basic-collection-object <?php print $value['class']; ?>">
        <dt class="islandora-basic-collection-thumb"><?php print $value['thumb_link']; ?></dt>
        <dd class="islandora-basic-collection-caption"><?php print filter_xss($value['title_link']); ?></dd>
        <dd class="collection-value transcribe-btns">
        <?php if (isset($value['transcribe_tpen_btn'])): ?>
          <?php print ($value['transcribe_tpen_btn']); ?>
        <?php endif; ?>
        <?php if (isset($value['master_tpen_btn'])): ?>
          <?php print ($value['master_tpen_btn']); ?>
        <?php endif; ?>
        </dd>
    </dl>
  <?php endforeach; ?>
</div>
</div>
