using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Enemy : MonoBehaviour
{
    public float lookRadius = 3f;
    public int damage = 10;
    public float attackSpeed = 1f;
    public float attackCooldown = 0f;
    public int maxHealth = 100;

    const float combatCooldown = 5f;

    float lastAttackTime;
    public bool InCombat{get;private set;}

    public int currentHealth{get;private set;}

    public float attackDelay = 1f;
    PlayerManager playerManager;

    public event System.Action<int,int> OnHealthChanged;

    public event System.Action OnAttack;

    void Awake(){
        currentHealth = maxHealth;
    }
    void Start(){
        playerManager = PlayerManager.instance;
    }

    void Update(){
        float distance = Vector3.Distance(playerManager.player.transform.position,transform.position);
        if(distance <= lookRadius){
            Interact();
        }
        attackCooldown -= Time.deltaTime;

        if(Time.time - lastAttackTime > combatCooldown){
            InCombat = false;
        }
    }

    void Interact(){
        PlayerHP hp = playerManager.player.GetComponent<PlayerHP>();
        if(hp != null){
            if(attackCooldown <= 0f){
                StartCoroutine(DoDamage(hp));
                if(OnAttack != null){
                    OnAttack();
                }
                attackCooldown = 1f/attackSpeed;

                InCombat = true;
                lastAttackTime = Time.time;
            }
        }
    }

    IEnumerator DoDamage(PlayerHP hp){
        yield return new WaitForSeconds(attackDelay);
        hp.updateHP(-damage);

        if(hp.getHealthPer() <= 0){
            InCombat = false;
        }
    }

    void OnDrawGizmosSelected(){
        Gizmos.color = Color.yellow;
        Gizmos.DrawWireSphere(transform.position,lookRadius);
    }

    public void TakeDamage(int damage){
        damage = Mathf.Clamp(damage,0,int.MaxValue);
        currentHealth -= damage;

        if (OnHealthChanged != null){
            OnHealthChanged(maxHealth,currentHealth);
        }

        if(currentHealth <= 0){
            Die();
        }
    }

    private void Die(){
        Destroy(this.gameObject);
    }
}
