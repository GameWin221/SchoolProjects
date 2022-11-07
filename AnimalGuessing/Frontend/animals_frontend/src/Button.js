import React from 'react'

export default function Button(props) {
    return(
        <input 
            type='button'
            value={props.caption}
            className={props.caption}
            onClick={props.onClick}/>
    )
}
