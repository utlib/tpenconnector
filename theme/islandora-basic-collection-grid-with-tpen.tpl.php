<?php

/**
 * @file
 * islandora-basic-collection-grid-with-tpen.tpl.php
 *
 * uses islandora-basic-collection-grid.tpl.php as the basis, but added a logic to include transcribe tpen & master tpen project buttons
 */
?>

<div class="islandora islandora-basic-collection">
  <div class="islandora-basic-collection-grid clearfix">
  <?php foreach($associated_objects_array as $key => $value): ?>
    <dl class="islandora-basic-collection-object <?php print $value['class']; ?>">
        <dt class="islandora-basic-collection-thumb"><?php print $value['thumb_link']; ?></dt>
        <dd class="islandora-basic-collection-caption"><?php print filter_xss($value['title_link']); ?></dd>
        <dd class="islandora-basic-collection-transcribe-btns">
        <ul>
        <?php if (isset($value['transcribe_tpen_btn'])): ?>
          <li><?php print ($value['transcribe_tpen_btn']); ?></li>
        <?php endif; ?>
        <?php if (isset($value['master_tpen_btn'])): ?>
          <li><?php print ($value['master_tpen_btn']); ?></li>
        <?php endif; ?>
        </ul>
        </dd>
    </dl>
  <?php endforeach; ?>
</div>
</div>
