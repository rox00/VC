using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;

namespace UYouMain
{
    [DataContract]
    public class Keys
    {
        [DataMember(Order = 0)]
        public double Key = 0;
        [DataMember(Order=1)]
        public double[] Position = new double[2] { 0, 0 };
    }

    [DataContract]
    public class Direction
    {
        [DataMember(Order = 0)]
        public double[] Center = new double[2] { 0, 0 };
        [DataMember(Order = 1)]
        public System.Collections.ArrayList Arrows = new System.Collections.ArrayList();
        public Direction()
        {
            Arrows.Add(new Keys());
            Arrows.Add(new Keys());
            Arrows.Add(new Keys());
            Arrows.Add(new Keys());
        }
    }

    [DataContract]
    public class KeyMappingConfig
    {
        [DataMember(Order = 0)]
        public Direction Direction = new Direction();
        [DataMember(Order = 1)]
        public System.Collections.ArrayList Mapping = new System.Collections.ArrayList();
        //public Keys[] Mapping = new Keys[2] { new Keys(), new Keys() };
            
        public KeyMappingConfig()
        {
            if (Direction == null)
            {
                Direction = new Direction();
            }
            Mapping.Add(new Keys());
            Mapping.Add(new Keys());
        }
    }
}
