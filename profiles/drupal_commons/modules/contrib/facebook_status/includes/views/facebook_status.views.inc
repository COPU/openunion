<?php

/**
 * @file
 * Provide views data and handlers for the Facebook-style Statuses module.
 */

/**
 * Implementation of hook_views_data().
 */
function facebook_status_views_data() {
  // Basic table information.
  $data['facebook_status']['table']['group']  = t('Facebook-style Statuses');
  $data['facebook_status']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'sender',
      'type' => 'INNER',
    ),
  );
  $data['users']['table']['join'] = array(
    'facebook_status' => array(
      'left_field' => 'sender',
      'field' => 'uid',
      'type' => 'INNER',
    ),
  );

  // Advertise this table as a possible base table.
  $data['facebook_status']['table']['base'] = array(
    'field' => 'sid',
    'title' => t('Facebook-style Statuses Updates'),
    'help' => t('Stores status updates.'),
    'weight' => 10,
  );

  // Make it possible to add a relationship to the Users table.
  // Commented out because Views automatically JOINs on the {users} table when
  // using User data without a Relationship, so adding the Relationship will
  // cause a duplicate JOIN.
  /*
  $data['facebook_status']['users'] = array(
    'group' => t('Facebook-style Statuses'),
    'relationship' => array(
      'title' => t('Users'),
      'label' => t('Users'),
      'help' => t('Add a relationship to gain access to information related to the users who submitted the relevant statuses.'),
      'relationship table' => 'facebook_status',
      'relationship field' => 'sender',
      'base' => 'users',
      'field' => 'uid',
      'type' => 'INNER',
      'handler' => 'views_handler_relationship',
    ),
  );
   */

  // Declares the Status ID column.
  $data['facebook_status']['sid'] = array(
    'title' => t('Status ID'),
    'help' => t('The ID of the status update.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Alias for Status ID to extend its use.
  $data['facebook_status']['last_per_user'] = array(
    'title' => t('Last Status per User'),
    'help' => t('Show only the last status update for each user.'),
    'filter' => array(
      'field' => 'sid',
      'handler' => 'facebook_status_views_handler_filter_latest_only',
      'label' => t('Last Status per User'),
    ),
  );
  $data['facebook_status']['edit'] = array(
    'title' => t('Edit'),
    'help' => t('Shows a link to edit the status to users with permission to see it.'),
    'field' => array(
      'field' => 'sid',
      'handler' => 'facebook_status_views_handler_field_edit',
      'click sortable' => FALSE,
    ),
  );
  $data['facebook_status']['delete'] = array(
    'title' => t('Delete'),
    'help' => t('Shows a link to delete the status to users with permission to see it.'),
    'field' => array(
      'field' => 'sid',
      'handler' => 'facebook_status_views_handler_field_delete',
      'click sortable' => FALSE,
    ),
  );
  $data['facebook_status']['repost'] = array(
    'title' => t('Re-post'),
    'help' => t('Shows a link to re-post a status.'),
    'field' => array(
      'field' => 'sid',
      'handler' => 'facebook_status_views_handler_field_repost',
      'click sortable' => FALSE,
    ),
  );

  // Declares the Type column.
  $data['facebook_status']['type'] = array(
    'title' => t('Recipient type'),
    'help' => t('The type of the recipient of the status message.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'facebook_status_views_handler_filter_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  $data['facebook_status']['auto-type'] = array(
    'title' => t('Use current context'),
    'help' => t('Only show status updates that match the current context (i.e. that appear in the stream that belongs to the entity the user is viewing).'),
    'filter' => array(
      'field' => 'type',
      'handler' => 'facebook_status_views_handler_filter_autotype',
    ),
  );

  // Declares the Recipient ID column.
  $data['facebook_status']['recipient'] = array(
    'title' => t('Recipient ID'),
    'help' => t('The ID of the recipient of the status.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Alias for Recipient ID to use the name.
  $data['facebook_status']['recipient_name'] = array(
    'title' => t('Recipient Name'),
    'help' => t('The name of the entity on whose stream the status message was posted.'),
    'field' => array(
      'field' => 'recipient',
      'handler' => 'facebook_status_views_handler_field_recipient',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'field' => 'recipient',
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'field' => 'recipient',
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'field' => 'recipient',
      'handler' => 'views_handler_argument_user_uid',
    ),
  );
  // Alias for poster picture.
  $data['facebook_status']['recipient_pic'] = array(
    'title' => t('Recipient picture'),
    'help' => t('The picture of the user who received the status message, if applicable.'),
    'field' => array(
      'field' => 'recipient',
      'handler' => 'facebook_status_views_handler_field_recipient_pic',
      'click sortable' => FALSE,
    ),
  );

  // Declares the Sender ID column.
  $data['facebook_status']['sender'] = array(
    'title' => t('Sender ID'),
    'help' => t('The User ID of the creator of the status.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  // Alias for extras for the sender.
  $data['facebook_status']['user_contextual'] = array(
    'title' => t('Usernames (context-aware)'),
    'help' => t('The themed username(s) of either the owner or the sender and recipient of the status, depending on the context.'),
    'field' => array(
      'field' => 'sender',
      'handler' => 'facebook_status_views_handler_field_cross',
      'click sortable' => FALSE,
      'label' => t('Users'),
    ),
  );
  $data['facebook_status']['not_own'] = array(
    'title' => t('Not own statuses'),
    'help' => t("Do not show statuses posted to one's own profile."),
    'filter' => array(
      'field' => 'sender',
      'handler' => 'facebook_status_views_handler_filter_not_own',
      'label' => t("Exclude own status updates"),
    ),
  );
  // This is slightly different than, but could probably be combined with, not_own.
  $data['facebook_status']['only_own'] = array(
    'title' => t('Only own statuses'),
    'help' => t("Only show statuses posted to one's own profile."),
    'filter' => array(
      'field' => 'sender',
      'handler' => 'facebook_status_views_handler_filter_own',
      'label' => t('Restrict to own status updates'),
    ),
  );
  $data['facebook_status']['sender_pic'] = array(
    'title' => t('Sender picture'),
    'help' => t('The picture of the user who sent the status message.'),
    'field' => array(
      'field' => 'sender',
      'handler' => 'facebook_status_views_handler_field_sender_pic',
      'click sortable' => FALSE,
    ),
  );
  $data['facebook_status']['user_contextual_pic'] = array(
    'title' => t('Users with Pictures'),
    'help' => t('The themed username(s) and pictures of either the owner or the sender and recipient of the status, depending on the context.'),
    'field' => array(
      'field' => 'sender',
      'handler' => 'facebook_status_views_handler_field_cross_pic',
      'click sortable' => FALSE,
      'label' => t('Users with Pictures'),
    ),
  );
  $data['facebook_status']['respond'] = array(
    'title' => t('Respond link'),
    'help' => t('Links to view the conversation or respond to it when appropriate.'),
    'field' => array(
      'field' => 'sender',
      'handler' => 'facebook_status_views_handler_field_respond',
      'click sortable' => FALSE,
      'label' => t('Respond to conversation'),
    ),
  );

  // Adds an argument and filter for "is participant."
  $data['facebook_status']['participant'] = array(
    'title' => t('User is sender or recipient'),
    'help' => t('Limits status messages to those where the specified user ID belongs to either the sender or recipient.'),
    'filter' => array(
      'field' => 'sender',
      'handler' => 'facebook_status_views_handler_filter_participant',
      'label' => t('User is participant'),
    ),
    'argument' => array(
      'field' => 'sender',
      'handler' => 'facebook_status_views_handler_argument_participant',
      'label' => t('User is participant'),
    ),
  );

  // Declares the status message timestamp column.
  $data['facebook_status']['created'] = array(
    'title' => t('Created time'),
    'help' => t('The time the status message was posted.'),
    'field' => array(
      'handler' => 'facebook_status_views_handler_field_created',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'argument' => array(
      'handler' => 'facebook_status_views_handler_argument_created',
      'field' => 'created',
      'title' => t('Created time (YYYYMMDD)'),
    ),
  );

  // Declares the status text column.
  $data['facebook_status']['message'] = array(
    'title' => t('Status message'),
    'help' => t('The text of the status update.'),
    'field' => array(
      'handler' => 'facebook_status_views_handler_field_message',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  $data['facebook_status']['classes'] = array(
    'title' => t('HTML classes'),
    'help' => t('The HTML classes that should be associated with a status.'),
    'field' => array(
      'field' => 'sender',
      'handler' => 'facebook_status_views_handler_field_classes',
      'click sortable' => FALSE,
    ),
  );

  // Limits users by who they have communicated with.
  $data['users']['communicated'] = array(
    'title' => t('Users have communicated with current user'),
    'help' => t('Shows only users who have sent status messages to or received status messages from the current user.'),
    'filter' => array(
      'field' => 'uid',
      'handler' => 'facebook_status_views_handler_filter_communicated',
      'label' => t('Users communicated'),
    ),
  );
  $data['users']['communicate'] = array(
    'title' => t('Users have communicated with argument user'),
    'help' => t('Shows only users who have sent status messages to or received status messages from the argument user.'),
    'argument' => array(
      'field' => 'uid',
      'handler' => 'facebook_status_views_handler_argument_communicated',
      'label' => t('Users communciated'),
    ),
  );

  // Last status to current user from user.
  $data['users']['last_status'] = array(
    'title' => t('Last status to current user'),
    'help' => t('Shows the last status each user sent to the current user, if any.'),
    'field' => array(
      'field' => 'uid',
      'handler' => 'facebook_status_views_handler_field_last',
    ),
  );

  // Current user's UID.
  $data['users']['current_uid'] = array(
    'title' => t("Current user's User ID"),
    'help' => t('Shows the User ID of the current user.'),
    'field' => array(
      'field' => 'uid',
      'handler' => 'facebook_status_views_handler_field_current_uid',
    ),
  );

  if (module_exists('user_relationships_api')) {
    // Expose data from a user's friends AND that user.
    $data['user_relationships']['rels_and_me'] = array(
      'title' => t('Requestee or Requester or Argument user'),
      'help' => t('Filters results to show content for the requestee, requester, or argument user.'),
      'argument' => array(
        'field' => 'requestee_id',
        'handler' => 'facebook_status_views_handler_argument_rels_and_me',
      ),
    );
  }
  
  if (module_exists('og_views')) {
    // Include statuses from a user's groups.
    $data['facebook_status']['og_statuses'] = array(
      'title' => t('Show group statuses'),
      'filter' => array(
        'field' => 'type',
        'handler' => 'facebook_status_views_handler_filter_og',
        'help' => t("Limits statuses to those in the current user's stream and in the current user's groups' streams."),
      ),
      'argument' => array(
        'field' => 'sender',
        'handler' => 'facebook_status_views_handler_argument_og',
        'help' => t("Limits statuses to those in the argument user's stream and in the argument user's groups' streams."),
      ),
    );
  }

  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function facebook_status_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'facebook_status') .'/includes/views/handlers',
    ),
    'handlers' => array(
      'facebook_status_views_handler_argument_communicated' => array(
        'parent' => 'views_handler_argument',
      ),
      'facebook_status_views_handler_argument_created' => array(
        'parent' => 'views_handler_argument_date',
      ),
      'facebook_status_views_handler_argument_og' => array(
        'parent' => 'views_handler_argument',
      ),
      'facebook_status_views_handler_argument_participant' => array(
        'parent' => 'views_handler_argument',
      ),
      'facebook_status_views_handler_argument_rels_and_me' => array(
        'parent' => 'views_handler_argument',
      ),
      'facebook_status_views_handler_field_classes' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_created' => array(
        'parent' => 'views_handler_field_date',
      ),
      'facebook_status_views_handler_field_cross' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_cross_pic' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_current_uid' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_delete' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_edit' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_last' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_message' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_recipient' => array(
        'parent' => 'views_handler_field_user',
      ),
      'facebook_status_views_handler_field_recipient_pic' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_repost' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_respond' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_field_sender_pic' => array(
        'parent' => 'views_handler_field',
      ),
      'facebook_status_views_handler_filter_autotype' => array(
        'parent' => 'views_handler_filter',
      ),
      'facebook_status_views_handler_filter_communicated' => array(
        'parent' => 'views_handler_filter',
      ),
      'facebook_status_views_handler_filter_latest_only' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'facebook_status_views_handler_filter_og' => array(
        'parent' => 'views_handler_filter',
      ),
      'facebook_status_views_handler_filter_participant' => array(
        'parent' => 'views_handler_filter',
      ),
      'facebook_status_views_handler_filter_not_own' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'facebook_status_views_handler_filter_own' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'facebook_status_views_handler_filter_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function facebook_status_views_plugins() {
  return array(
    'row' => array(
      'facebook_status_rss' => array(
        'title' => t('RSS items'),
        'help' => t('Displays statuses as RSS items.'),
        'handler' => 'facebook_status_views_plugin_row_rss',
        'parent' => 'fields',
        'path' => drupal_get_path('module', 'facebook_status') .'/includes/views/handlers',
        'theme' => 'views_view_row_rss',
        'base' => array('facebook_status'),
        'uses fields' => FALSE,
        'uses options' => FALSE,
        'type' => 'feed',
      ),
    ),
    'argument validator' => array(
      'status' => array(
        'title' => t('Status'),
        'handler' => 'facebook_status_plugin_argument_validate',
      ),
    ),
  );
}
