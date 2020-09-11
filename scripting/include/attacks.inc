#if defined _attacks_included
  #endinput
#endif
#define _attacks_included

enum Bombsite {
    BombsiteA = 0,
    BombsiteB = 1,
};

// Spawn types. These only apply to T-side spawns, all
// CT-spawns are considered SpawnType_Normal since they have no bomb.
enum SpawnType {
    SpawnType_Normal = 0,
    SpawnType_OnlyWithBomb = 1,
    SpawnType_NeverWithBomb = 2,
};

#define SITESTRING(%1) ((%1) == BombsiteA ? "A" : "B")
#define TEAMSTRING(%1) ((%1) == CS_TEAM_CT ? "CT" : "T")

/**
 * Maxmimum length of a nade string. Example: "hfs" is a hegrenade, flashbang, and smoke.
 */
#define NADE_STRING_LENGTH 8

/**
 * Maxmimum length of a weapon name. Example: "weapon_ak47"
 */
#define WEAPON_STRING_LENGTH 32

/**
 * Called right before players get put onto teams for the next round.
 * This is the best place to decide who goes onto what team if you want
 * to change the default behavior.
 *
 * @param rankingQueue a priority queue (see include/priorityqueue.inc)
 * @param waitingQueue a queue of the players waiting to join (see include/queue.inc)
 * @noreturn
 */
forward void Attacks_OnPreRoundEnqueue(ArrayList rankingQueue, ArrayList waitingQueue);

/**
 * Called after active players have been placed into the priority scoring queue
 * for the next round. This is a convenient place to change their scores by
 * editing the ranking priority queue itself.
 * (rather than using the Attacks_SetRoundPoints native)
 *
 * @param rankingQueue a priority queue (see include/priorityqueue.inc)
 * @param waitingQueue a queue of the players waiting to join (see include/queue.inc)
 * @noreturn
 */
forward void Attacks_OnPostRoundEnqueue(ArrayList rankingQueue, ArrayList waitingQueue);

/**
 * Called when the bombsite for the round is decided.
 *
 * @param site which bombsite the round will use
 * @noreturn
 */
forward void Attacks_OnSitePicked(Bombsite& site);

/**
 * Called when the team sizes are set for the round.
 *
 * @param tCount the number of terrorists that will play the round
 * @param ctcount the number of counter-terrorists that will play the round
 * @noreturn
 */
forward void Attacks_OnTeamSizesSet(int& tCount, int& ctCount);

/**
 * Called when a player fails to plant the bomb when he spawned with it.
 *
 * @param client the player that did not plant
 * @noreturn
 */
forward void Attacks_OnFailToPlant(int client);

/**
 * Called when a team wins a round.
 *
 * @param winner the winning team (CS_TEAM_T or CS_TEAM_CT)
 * @param tPlayers an ArrayList of the players on the terrorist team
 * @param ctPlayers an ArrayList of the players on the counter-terrorist team
 * @noreturn
 */
forward void Attacks_OnRoundWon(int winner, ArrayList tPlayers, ArrayList ctPlayers);

/**
 * Called after teams have been determined for the round.
 *
 * @param tPlayers an ArrayList of the players on the terrorist team
 * @param ctPlayers an ArrayList of the players on the counter-terrorist team
 * @param bombsite
 * @noreturn
 */
forward void Attacks_OnTeamsSet(ArrayList tPlayers, ArrayList ctPlayers, Bombsite bombsite);

/**
 * Called when player weapons are being allocated for the round.
 *
 * @param tPlayers an ArrayList of the players on the terrorist team
 * @param ctPlayers an ArrayList of the players on the counter-terrorist team
 * @param bombsite
 * @noreturn
 */
forward void Attacks_OnWeaponsAllocated(ArrayList tPlayers, ArrayList ctPlayers, Bombsite bombsite);

/**
 * Called when a client issues a command to bring up a "guns" menu.
 */
forward void Attacks_OnGunsCommand(int client);

/**
 * Returns if a player has joined the game, i.e., if they are on T/Ct or in the waiting queue.
 *
 * @param client a player
 * @return if the player has joined
 */
native bool Attacks_IsJoined(int client);

/**
 * Returns if a player is in the waiting queue.
 *
 * @param client a player
 * @return if the player is in the waiting queue
 */
native bool Attacks_IsInQueue(int client);

/**
 * Sends a retake formatted message to a client.
 *
 * @param client a player
 * @param format string message
 * @noreturn
 */
native void Attacks_Message(int client, const char[] format, any ...);

/**
 * Sends a retake formatted message to all clients.
 *
 * @param format string message
 * @noreturn
 */
native void Attacks_MessageToAll(const char[] format, any ...);

/**
 * Returns the number of terrorists for the current round.
 */
native int Attacks_GetNumActiveTs();

/**
 * Returns the number of terrorists for the current round.
 */
native int Attacks_GetNumActiveCTs();

/**
 * Returns the number of active players (t+ct) for the current round.
 */
native int Attacks_GetNumActivePlayers();

/**
 * Returns the bombsite for the current scenario.
 */
native Bombsite Attacks_GetCurrrentBombsite();

/**
 * Returns the round points for a client in the current round.
 */
native int Attacks_GetRoundPoints(int client);

/**
 * Sets the round points for a client in the current round.
 */
native int Attacks_SetRoundPoints(int client, int points);

/**
 * Changes the round points for a client in the current round.
 */
native void Attacks_ChangeRoundPoints(int client, int dp);

/**
 * Sets player weapon/equipment information for the current round.
 */
native void Attacks_SetPlayerInfo(int client,
                                  const char[] primary="",
                                  const char[] secondary="",
                                  const char[] nades="",
                                  int health=100,
                                  int armor=0,
                                  bool helmet=false,
                                  bool kit=false);

/**
 * Gets player weapon/equipment information for the current round.
 */
native void Attacks_GetPlayerInfo(int client,
                                  char primary[WEAPON_STRING_LENGTH],
                                  char secondary[WEAPON_STRING_LENGTH],
                                  char nades[NADE_STRING_LENGTH],
                                  int& health,
                                  int& armor,
                                  bool& helmet,
                                  bool& kit);

/**
 * Returns the total number of live rounds played on the current map.
 */
native int Attacks_GetRetakeRoundsPlayed();

/**
 * Returns if edit mode is active.
 */
native bool Attacks_InEditMode();

/**
 * Returns if the game is currently in a warmup phase.
 */
native bool Attacks_InWarmup();

/**
 * Returns if the plugin is enabled.
 */
native bool Attacks_Enabled();

/**
 * Returns if the plugin is enabled and not in warmup.
 */
stock bool Attacks_Live() {
    return Attacks_Enabled() && !Attacks_InWarmup() && !Attacks_InEditMode();
}

/**
 * Returns the maximum number of players allowed into the game.
 */
native int Attacks_GetMaxPlayers();

public SharedPlugin __pl_attacks = {
    name = "attacks",
    file = "attacks.smx",
#if defined REQUIRE_PLUGIN
    required = 1,
#else
    required = 0,
#endif
};

#if !defined REQUIRE_PLUGIN
public __pl_attacks_SetNTVOptional() {
    MarkNativeAsOptional("Attacks_IsJoined");
    MarkNativeAsOptional("Attacks_IsInQueue");
    MarkNativeAsOptional("Attacks_Message");
    MarkNativeAsOptional("Attacks_MessageToAll");
    MarkNativeAsOptional("Attacks_GetNumActiveTs");
    MarkNativeAsOptional("Attacks_GetNumActiveCTs");
    MarkNativeAsOptional("Attacks_GetNumActivePlayers");
    MarkNativeAsOptional("Attacks_GetCurrrentBombsite");
    MarkNativeAsOptional("Attacks_GetRoundPoints");
    MarkNativeAsOptional("Attacks_SetRoundPoints");
    MarkNativeAsOptional("Attacks_ChangeRoundPoints");
    MarkNativeAsOptional("Attacks_SetPlayerInfo");
    MarkNativeAsOptional("Attacks_GetPlayerInfo");
    MarkNativeAsOptional("Attacks_GetRetakeRoundsPlayed");
    MarkNativeAsOptional("Attacks_InEditMode");
    MarkNativeAsOptional("Attacks_InWarmup");
    MarkNativeAsOptional("Attacks_Enabled");
    MarkNativeAsOptional("Attacks_GetMaxPlayers");
}
#endif
