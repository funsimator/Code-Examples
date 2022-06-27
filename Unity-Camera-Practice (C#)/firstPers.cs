using System.Collections;
using System.Collections.Generic;
using UnityEngine.Animations;
using UnityEngine;

// First Person Camera Class

public class firstPers : MonoBehaviour
{
    public Transform kopf;
    private Quaternion kopfAusgangsPose;
    private Quaternion koerperAusgangsPose;

    private void Start()
    {
        kopfAusgangsPose = kopf.localRotation;
        koerperAusgangsPose = this.transform.localRotation;
        //transform.position = kopf.position;
    }

    public float winkelProSec = 90f;
    private float xWinkel = 0f;
    private float yWinkel = 0f;

    private void OnEnable()
    {
        xWinkel = 0f;
        yWinkel = 0f;
    }

    private void Update()
    {
        float horz = Input.GetAxis("Mouse X") * Time.deltaTime * winkelProSec;
        float vert = Input.GetAxis("Mouse Y") * Time.deltaTime * winkelProSec;

        yWinkel -= vert;
        xWinkel += horz;

        xWinkel = Mathf.Clamp(xWinkel, -45, 45);
        yWinkel = Mathf.Clamp(yWinkel, -45, 45);

        kopf.transform.localRotation = Quaternion.AngleAxis(yWinkel, Vector3.right) * kopfAusgangsPose;
        this.transform.localRotation = Quaternion.AngleAxis(xWinkel, Vector3.up) * koerperAusgangsPose ;

        //Vector3 dsa = kopf.transform.localRotation * Vector3.up;
        //this.transform.rotation = koerperAusgangsPose * Quaternion.AngleAxis(xWinkel, Vector3.up) * Quaternion.AngleAxis(yWinkel, Vector3.right);

       

    }
}